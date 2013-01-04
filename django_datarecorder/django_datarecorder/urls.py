from django.conf.urls import patterns, include, url
from django.conf.urls.defaults import *
from recorder.views import *
import os.path

site_media = os.path.join(
    os.path.dirname(__file__), '../site_media' 
)

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()



from django.views.generic.simple import direct_to_template

urlpatterns = patterns('',
    url(r'^$', 'recorder.views.main_page', name='main_page'),
        
    # Session Management
    (r'^login/$', 'django.contrib.auth.views.login'),
    (r'^shutdown/$', shutdown_page),
    (r'^logout/$', logout_page),
    (r'^site_media/(?P<path>.*)$', 'django.views.static.serve', 
        { 'document_root': site_media }),
    (r'^register/$', register_page),
    (r'^register/success/$', direct_to_template,
        {'template': 'registration/register_success.html'}),
    (r'^user/(\w+)/$', user_page),
                       
    # Uncomment the admin/doc line below to enable admin documentation:
    url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    url(r'^admin/', include(admin.site.urls)),
)
