# Create your views here.
from dajax.core import Dajax
from django.shortcuts import render_to_response
from django.template import Context, RequestContext
from recorder.forms import RegistrationForm

from django.http import HttpResponseRedirect
from django.contrib.auth import logout
from django.contrib.auth.models import User

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
    
def alert_example(request):
    dajax = Dajax()
    dajax.alert('Hello from python!')
    return dajax.json()

def main_page(request):
    global test
    test = test +1
    variables = Context({
        'test': alert_example(request),
    })
    return render_to_response('main_page.html', variables)        

test = 0