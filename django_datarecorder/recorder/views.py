# Create your views here.
from dajax.core import Dajax
from django.shortcuts import render_to_response
from django.template import Context

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