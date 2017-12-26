from django.conf.urls import url,include
from django.contrib import admin
from . import views
urlpatterns = [
    url(r'^mypage/$',views.mypage),
    url(r'^new/$',views.make_new),
    url(r'^main/$',views.market_main),
    url(r'^btc/$', views.market_btc),
    url(r'^eth/$', views.market_eth),
    url(r'^bch/$', views.market_bch),
    url(r'^xrp/$', views.market_xrp),
    url(r'^xmr/$', views.market_xmr),
    url(r'^zec/$', views.market_zec),
    url(r'^btg/$', views.market_btg),
    url(r'^etc/$', views.market_etc),
    url(r'^eos/$', views.market_eos),
    url(r'^ltc/$', views.market_ltc),
    url(r'^qtum/$', views.market_qtum),
    url(r'^dash/$', views.market_dash),
    url(r'^pixel/$', views.market_pixel),
]
