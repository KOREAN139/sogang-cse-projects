from django.conf.urls import url

from user.views import login, login_validate, join_page, about

urlpatterns = [
    url(r'^login/$', login),
    url(r'^login/validate/$', login_validate),
    url(r'^join/$', join_page),
    url(r'^about/$', about),
]