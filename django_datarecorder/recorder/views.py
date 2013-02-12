# Create your views here.
from dajax.core import Dajax
from django.shortcuts import render_to_response
from django.template import Context, RequestContext
from recorder.forms import RegistrationForm
from django.shortcuts import get_object_or_404

from django.http import HttpResponseRedirect
from django.contrib.auth import logout
from django.contrib.auth.models import User
from recorder.models import EnvironmentModel
from django.core.context_processors import csrf
from xenomai_interface.recordercontroller import RecorderController
from xenomai_interface.ringbuffer import RingBuffer

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

def main_page(request):
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
        sensorControlForm = SensorControlForm(request.POST)
    else: 
        recControlForm = RecorderControlForm()
        sensorControlForm = SensorControlForm()
        
    environmentModel = EnvironmentModel.objects.latest("id");
    recordingIsActive = environmentModel.globalRecordingIsActive;
    
    systemtime = time.asctime(time.localtime())
    rbuf = RingBuffer()
    #rbufdata = '<h1>hallo welt</h1>'
    #rbufdata = rbuf.get_json()
    rbufdata = rbuf.get_html_table()
    
  	#time.strftime("%d.%m.%Y um %H:%M:%S Uhr")
    variables = Context({
    										 'systemtime': systemtime,
    										 'rbufdata': rbufdata,
                         'recordingIsActive': recordingIsActive,
                         'user': request.user
    });
    
    variables.update(csrf(request));
    return render_to_response('main_page.html', variables)    
    #return render_to_response('main_page.html', RequestContext(request))
#    global test
#    test = test +1
#    variables = Context({
#        'test': alert_example(request),
#    })
#    return render_to_response('main_page.html', variables)        

test = 0
