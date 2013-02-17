# Create your views here.
from dajax.core import Dajax
from django.shortcuts import render_to_response
from django.template import Context, RequestContext
from recorder.forms import RegistrationForm
from django.shortcuts import get_object_or_404

from django.http import HttpResponseRedirect, HttpResponse,\
    HttpResponseServerError, HttpResponseBadRequest
from django.contrib.auth import logout
from django.contrib.auth.models import User
from recorder.models import EnvironmentModel
from django.core.context_processors import csrf
from xenomai_interface.recordercontroller import RecorderController
from xenomai_interface.read_shm import ReadRingBuffer

import os, time

def logout_page(request):
    logout(request)
    return HttpResponseRedirect('/')

from recorder.forms import *
def register_page(request):
    if request.method == 'POST':
        form = RegistrationForm(request.POST)
        if form.is_valid():
            user = User.objects.create_user(
                username=form.cleaned_data['username'],
                password=form.cleaned_data['password1'],
                email=form.cleaned_data['email']
            )
            return HttpResponseRedirect('/register/success/')
    else:
        form = RegistrationForm()
    variables = RequestContext(request, {
            'form': form
        })
    return render_to_response(
        'registration/register.html',
        variables
    )
    
def user_page(request, username):
    user = get_object_or_404(User, username=username)
    variables = Context({
        'user': request.user,
        'username': username,
    })

    return render_to_response('user_page.html', variables)
    
def shutdown_page(request):
	os.system("sudo halt")
	return render_to_response('shutdown_page.html')

def alert_example(request):
    dajax = Dajax()
    dajax.alert('Hello from python!')
    return dajax.json()

def newest_data_page(request, maxlength = 100):
    rbuf = ReadRingBuffer()
    rbuf.setLastDataMode(long(maxlength))
    rbufdata = rbuf.get_html_table();
    max_length_str = str(maxlength)
    variables = Context({
        'max_length': max_length_str,
        'rbufdata': rbufdata,
        'user': request.user
    })
    variables.update(csrf(request))

    return render_to_response('new_data_page.html', variables)

def newest_data_png_page(request, maxlength = 100):
    rbuf = ReadRingBuffer()
    rbuf.setLastDataMode(long(maxlength))
    rbufdata = rbuf.get_png()
    response = HttpResponse(rbufdata, content_type='image/png')
    response['Cache-Control'] = 'no-cache'
    #response['Content-Disposition'] = 'attachment; filename="ringbufferdata.png"'
    return response
    
def main_page(request):
    rb = RingBufferConsumer()
    rb.init()
    if request.method == 'POST':    
        recorderController = RecorderController()
        recordControl = False
        if 'record_true' in request.POST:
            recordingRequest = True
            recordControl = True
            recorderController.start_record()
        elif 'record_false' in request.POST:
            recordingRequest = False
            recordControl = True
            recorderController.pause_record()
        if recordControl:
            recControlForm = RecorderControlForm(request.POST)
            #print "Test"
            #if recControlForm.is_valid():
            #    print "Valid"
            environmentModel = EnvironmentModel.objects.create(globalRecordingIsActive=recordingRequest, realTimeViolationCounter=0)
            environmentModel.save();
        
        if 'update_sensor_control' in request.POST:
            sensorControlForm = SensorControlForm(request.POST)
            if sensorControlForm.is_valid():
                samplingRate = sensorControlForm.cleaned_data['samplingRate']
                recorderController.speed(samplingRate)
                ringBufferSize = sensorControlForm.cleaned_data['ringBufferSize']
                rb.setSize(ringBufferSize)
                # TODO Wee need a Mutex here
                #rb.
            
    else: 
        recControlForm = RecorderControlForm()
        sensorControlForm = SensorControlForm()
    
    environmentModel = EnvironmentModel.objects.latest("id");
    recordingIsActive = environmentModel.globalRecordingIsActive;
    ringBufferSize = rb.getSize();
    
    systemtime = time.asctime(time.localtime())
    rbuf = ReadRingBuffer()
    rbuf.setLastDataMode(20)
    #rbufdata = '<h1>hallo welt</h1>'
    #rbufdata = rbuf.get_json()
    rbufdata = rbuf.get_html_table()
    
    xen_proc_stat_in = open("/proc/xenomai/stat", "r")
    systemstat = xen_proc_stat_in.read()
    
  	#time.strftime("%d.%m.%Y um %H:%M:%S Uhr")
    variables = Context({
    	'systemtime': systemtime,
        'systemstat': systemstat,
    	'rbufdata': rbufdata,
        'recordingIsActive': recordingIsActive,
        'ringBufferSize': ringBufferSize,
        'user': request.user
    });
    
    variables.update(csrf(request))
    return render_to_response('main_page.html', variables)    
    #return render_to_response('main_page.html', RequestContext(request))
#    global test
#    test = test +1
#    variables = Context({
#        'test': alert_example(request),
#    })
#    return render_to_response('main_page.html', variables)        

from xenomai_interface.RingBufferConsumer import RingBufferConsumer
from xenomai_interface.RingBufferPageIndex import RingBufferPageIndex 

def datanav_page(request, page_number, maxlength = 100):
    # types = "Page_number = %d" % long(page_number)
    #return HttpResponse(types)
    rbc = RingBufferConsumer()
    result = HttpResponseServerError("Couldn't connect to Datarecorder")
    if rbc.init():
        p = long(page_number)
        m = long(maxlength)
        rbpi = RingBufferPageIndex(rbc)
        # Fixme
        try:
            rbpi.setPage(p, m)
        except:
            return HttpResponseBadRequest("RingBuffer Page out of Range")
        count_pages = rbpi.getCountPages()
        rbuf = ReadRingBuffer()
        rbuf.setPageMode(p, m)
        rbufdata = rbuf.get_html_table()
        
        #first_next_page = 0
        #if page_number>5:
        #   first_next_page = page_number -5
            
        class page:
            def __init__(self):
                self.islink = False
                self.name = ""
                self.link = ""
        
        nextlinks = []
        for nextpage in range(0, count_pages):
            currentpage = page()
            if nextpage == page_number:
                currentpage.islink = False
            else:
                currentpage.islink = True
                currentpage.link = "/recorded_data/" + str(nextpage) + "/" + str(maxlength) + "/" 
            currentpage.name = str(nextpage)
            nextlinks.append(currentpage)
        
        
        #nextlink = "test"

        variables = Context({
            'rbufdata': rbufdata,
            'page_number': page_number,
            'maxlength': maxlength,
            'count_pages': count_pages,
        #  'nextlink': nextlink,
            'nextlinks': nextlinks,
            'user': request.user
        });
        variables.update(csrf(request));
        result = render_to_response('data_page.html', variables)
    
    return result

#