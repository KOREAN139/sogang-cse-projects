from django.shortcuts import render, redirect
from django.contrib.auth.models import User
from django.http.response import HttpResponse
from django.template.loader import get_template
from django.core.paginator import Paginator, PageNotAnInteger, EmptyPage
from django.template.context_processors import csrf
from django.views.generic import ListView,DetailView
from market.forms import *
from market.models import *
from market.crawl import *
from user.models import *
import datetime
import decimal
# Create your views here.

def mypage(request):
    if not request.user.is_authenticated:
        return redirect('/user/login/')
    template = get_template('mypage.html')
    obj = wallet.objects.get(Owner=request.user)
    col = pixel_table.objects.get(Owner=request.user)
    context = {'col':col,'wallet':obj, 'is_login':request.user.is_authenticated}

    return HttpResponse(template.render(context))

def make_new(request):
    wallet.objects.create(Owner=request.user, KRW=1000000, BTC = 0.0, ETH = 0.0, BCH = 0.0, XRP = 0.0, LTC = 0.0, DASH = 0.0, XMR = 0.0, BTG = 0.0,
    EOS = 0.0, QTUM = 0.0, ETC = 0.0, ZEC = 0.0)

    return redirect('/market/main')


def market_main(request):
    template = get_template('main.html')
    status = get_info('')
    price = {}
    pix = pixel_table.objects.all()
    for line in status:
        temp = format(int(float(line[0]['price_krw'])),',')
        price[line[0]['symbol']] = temp
        price[line[0]['symbol']+'p'] = float(line[0]['percent_change_24h'])

    context = {'pixel':pix,'price':price,'is_login':request.user.is_authenticated}
    return HttpResponse(template.render(context))

def init(request):
    return redirect('/market/main/')
# Create your views here.

def market_btc(request):
    status = get_info('bitcoin')
    price = status[0][0]
    price['24h_volume_krw'] = format(int(float(price['24h_volume_krw'])),',')
    price['price_krw'] = int(float(price['price_krw']))
    price['market_cap_krw'] = format(int(float(price['market_cap_krw'])), ',')
    if(price['max_supply']):
        price['max_supply'] = format(int(float(price['max_supply'])), ',')
    price['total_supply'] = format(int(float(price['total_supply'])), ',')
    pk = request.POST.get('pk', None)
    template = get_template('btc.html')
    form_data = btc_form()
    obj = wallet.objects.get(Owner=request.user)
    flag = 'none'

    if request.is_ajax():
        data = price['price_krw'] * float(request.GET['Amount'])
        import json
        return HttpResponse(json.dumps({'message':data}), 'application/json')
    if request.method == 'POST':
        form_data = btc_form(request.POST)
        if form_data.is_valid():
            if request.POST.get("buy"):
                btc = form_data.cleaned_data['BTC']

                if price['price_krw'] * btc < obj.KRW:
                    obj.KRW = int(obj.KRW) - int(float(price['price_krw']) * float(btc))
                    obj.BTC = obj.BTC + btc
                    obj.save()
                else:
                    print('error')
                    flag = 'buy'
            elif request.POST.get("sell"):
                btc = form_data.cleaned_data['BTC']
                if btc <= obj.BTC:
                    obj.KRW = int(obj.KRW) + int(float(price['price_krw']) * float(btc))
                    obj.BTC = obj.BTC - btc
                    obj.save()
                else:
                    print('error')
                    flag = 'sell'


    context = {'price':price,'form': form_data, 'wallet': obj, 'is_login': request.user.is_authenticated,'flag':flag}
    context.update(csrf(request))
    return HttpResponse(template.render(context))

def market_eth(request):
    status = get_info('ethereum')
    price = status[0][0]
    price['24h_volume_krw'] = format(int(float(price['24h_volume_krw'])),',')
    price['price_krw'] = int(float(price['price_krw']))
    price['market_cap_krw'] = format(int(float(price['market_cap_krw'])), ',')
    if (price['max_supply']):
        price['max_supply'] = format(int(float(price['max_supply'])), ',')
    price['total_supply'] = format(int(float(price['total_supply'])), ',')
    pk = request.POST.get('pk', None)
    template = get_template('eth.html')
    form_data = eth_form()
    obj = wallet.objects.get(Owner=request.user)
    flag = 'none'

    if request.is_ajax():
        data = price['price_krw'] * float(request.GET['Amount'])
        import json
        return HttpResponse(json.dumps({'message':data}), 'application/json')
    if request.method == 'POST':
        form_data = eth_form(request.POST)
        if form_data.is_valid():
            if request.POST.get("buy"):
                btc = form_data.cleaned_data['ETH']

                if price['price_krw'] * btc < obj.KRW:
                    obj.KRW = int(obj.KRW) - int(float(price['price_krw']) * float(btc))
                    obj.ETH = obj.ETH + btc
                    obj.save()
                else:
                    print('error')
                    flag = 'buy'
            elif request.POST.get("sell"):
                btc = form_data.cleaned_data['ETH']
                if btc <= obj.ETH:
                    obj.KRW = int(obj.KRW) + int(float(price['price_krw']) * float(btc))
                    obj.ETH = obj.ETH - btc
                    obj.save()
                else:
                    print('error')
                    flag = 'sell'


    context = {'price':price,'form': form_data, 'wallet': obj, 'is_login': request.user.is_authenticated,'flag':flag}
    context.update(csrf(request))
    return HttpResponse(template.render(context))

def market_bch(request):
    status = get_info('bitcoin-cash')
    price = status[0][0]
    price['24h_volume_krw'] = format(int(float(price['24h_volume_krw'])),',')
    price['price_krw'] = int(float(price['price_krw']))
    price['market_cap_krw'] = format(int(float(price['market_cap_krw'])), ',')
    if (price['max_supply']):
        price['max_supply'] = format(int(float(price['max_supply'])), ',')
    price['total_supply'] = format(int(float(price['total_supply'])), ',')
    pk = request.POST.get('pk', None)
    template = get_template('bch.html')
    form_data = bch_form()
    obj = wallet.objects.get(Owner=request.user)
    flag = 'none'

    if request.is_ajax():
        data = price['price_krw'] * float(request.GET['Amount'])
        import json
        return HttpResponse(json.dumps({'message':data}), 'application/json')
    if request.method == 'POST':
        form_data = bch_form(request.POST)
        if form_data.is_valid():
            if request.POST.get("buy"):
                btc = form_data.cleaned_data['BCH']

                if price['price_krw'] * btc < obj.KRW:
                    obj.KRW = int(obj.KRW) - int(float(price['price_krw']) * float(btc))
                    obj.BCH = obj.BCH + btc
                    obj.save()
                else:
                    print('error')
                    flag = 'buy'
            elif request.POST.get("sell"):
                btc = form_data.cleaned_data['BCH']
                if btc <= obj.BCH:
                    obj.KRW = int(obj.KRW) + int(float(price['price_krw']) * float(btc))
                    obj.BCH = obj.BCH - btc
                    obj.save()
                else:
                    print('error')
                    flag = 'sell'


    context = {'price':price,'form': form_data, 'wallet': obj, 'is_login': request.user.is_authenticated,'flag':flag}
    context.update(csrf(request))
    return HttpResponse(template.render(context))

def market_xrp(request):
    status = get_info('ripple')
    price = status[0][0]
    price['24h_volume_krw'] = format(int(float(price['24h_volume_krw'])),',')
    price['price_krw'] = int(float(price['price_krw']))
    price['market_cap_krw'] = format(int(float(price['market_cap_krw'])), ',')
    if (price['max_supply']):
        price['max_supply'] = format(int(float(price['max_supply'])), ',')
    price['total_supply'] = format(int(float(price['total_supply'])), ',')
    pk = request.POST.get('pk', None)
    template = get_template('xrp.html')
    form_data = xrp_form()
    obj = wallet.objects.get(Owner=request.user)
    flag = 'none'

    if request.is_ajax():
        data = price['price_krw'] * float(request.GET['Amount'])
        import json
        return HttpResponse(json.dumps({'message':data}), 'application/json')
    if request.method == 'POST':
        form_data = xrp_form(request.POST)
        if form_data.is_valid():
            if request.POST.get("buy"):
                btc = form_data.cleaned_data['XRP']

                if price['price_krw'] * btc < obj.KRW:
                    obj.KRW = int(obj.KRW) - int(float(price['price_krw']) * float(btc))
                    obj.XRP = obj.XRP + btc
                    obj.save()
                else:
                    print('error')
                    flag = 'buy'
            elif request.POST.get("sell"):
                btc = form_data.cleaned_data['XRP']
                if btc <= obj.XRP:
                    obj.KRW = int(obj.KRW) + int(float(price['price_krw']) * float(btc))
                    obj.XRP = obj.XRP - btc
                    obj.save()
                else:
                    print('error')
                    flag = 'sell'


    context = {'price':price,'form': form_data, 'wallet': obj, 'is_login': request.user.is_authenticated,'flag':flag}
    context.update(csrf(request))
    return HttpResponse(template.render(context))

def market_ltc(request):
    status = get_info('litecoin')
    price = status[0][0]
    price['24h_volume_krw'] = format(int(float(price['24h_volume_krw'])),',')
    price['price_krw'] = int(float(price['price_krw']))
    price['market_cap_krw'] = format(int(float(price['market_cap_krw'])), ',')
    if (price['max_supply']):
        price['max_supply'] = format(int(float(price['max_supply'])), ',')
    price['total_supply'] = format(int(float(price['total_supply'])), ',')
    pk = request.POST.get('pk', None)
    template = get_template('ltc.html')
    form_data = ltc_form()
    obj = wallet.objects.get(Owner=request.user)
    flag = 'none'

    if request.is_ajax():
        data = price['price_krw'] * float(request.GET['Amount'])
        import json
        return HttpResponse(json.dumps({'message':data}), 'application/json')
    if request.method == 'POST':
        form_data = ltc_form(request.POST)
        if form_data.is_valid():
            if request.POST.get("buy"):
                btc = form_data.cleaned_data['LTC']

                if price['price_krw'] * btc < obj.KRW:
                    obj.KRW = int(obj.KRW) - int(float(price['price_krw']) * float(btc))
                    obj.LTC = obj.LTC + btc
                    obj.save()
                else:
                    print('error')
                    flag = 'buy'
            elif request.POST.get("sell"):
                btc = form_data.cleaned_data['LTC']
                if btc <= obj.LTC:
                    obj.KRW = int(obj.KRW) + int(float(price['price_krw']) * float(btc))
                    obj.LTC = obj.LTC - btc
                    obj.save()
                else:
                    print('error')
                    flag = 'sell'


    context = {'price':price,'form': form_data, 'wallet': obj, 'is_login': request.user.is_authenticated,'flag':flag}
    context.update(csrf(request))
    return HttpResponse(template.render(context))

def market_dash(request):
    status = get_info('dash')
    price = status[0][0]
    price['24h_volume_krw'] = format(int(float(price['24h_volume_krw'])),',')
    price['price_krw'] = int(float(price['price_krw']))
    price['market_cap_krw'] = format(int(float(price['market_cap_krw'])), ',')
    if (price['max_supply']):
        price['max_supply'] = format(int(float(price['max_supply'])), ',')
    price['total_supply'] = format(int(float(price['total_supply'])), ',')
    pk = request.POST.get('pk', None)
    template = get_template('dash.html')
    form_data = dash_form()
    obj = wallet.objects.get(Owner=request.user)
    flag = 'none'

    if request.is_ajax():
        data = price['price_krw'] * float(request.GET['Amount'])
        import json
        return HttpResponse(json.dumps({'message':data}), 'application/json')
    if request.method == 'POST':
        form_data = dash_form(request.POST)
        if form_data.is_valid():
            if request.POST.get("buy"):
                btc = form_data.cleaned_data['DASH']

                if price['price_krw'] * btc < obj.KRW:
                    obj.KRW = int(obj.KRW) - int(float(price['price_krw']) * float(btc))
                    obj.DASH = obj.DASH + btc
                    obj.save()
                else:
                    print('error')
                    flag = 'buy'
            elif request.POST.get("sell"):
                btc = form_data.cleaned_data['DASH']
                if btc <= obj.DASH:
                    obj.KRW = int(obj.KRW) + int(float(price['price_krw']) * float(btc))
                    obj.DASH = obj.DASH - btc
                    obj.save()
                else:
                    print('error')
                    flag = 'sell'


    context = {'price':price,'form': form_data, 'wallet': obj, 'is_login': request.user.is_authenticated,'flag':flag}
    context.update(csrf(request))
    return HttpResponse(template.render(context))

def market_xmr(request):
    status = get_info('monero')
    price = status[0][0]
    price['24h_volume_krw'] = format(int(float(price['24h_volume_krw'])),',')
    price['price_krw'] = int(float(price['price_krw']))
    price['market_cap_krw'] = format(int(float(price['market_cap_krw'])), ',')
    if (price['max_supply']):
        price['max_supply'] = format(int(float(price['max_supply'])), ',')
    price['total_supply'] = format(int(float(price['total_supply'])), ',')
    pk = request.POST.get('pk', None)
    template = get_template('monero.html')
    form_data = xmr_form()
    obj = wallet.objects.get(Owner=request.user)
    flag = 'none'

    if request.is_ajax():
        data = price['price_krw'] * float(request.GET['Amount'])
        import json
        return HttpResponse(json.dumps({'message':data}), 'application/json')
    if request.method == 'POST':
        form_data = xmr_form(request.POST)
        if form_data.is_valid():
            if request.POST.get("buy"):
                btc = form_data.cleaned_data['XMR']

                if price['price_krw'] * btc < obj.KRW:
                    obj.KRW = int(obj.KRW) - int(float(price['price_krw']) * float(btc))
                    obj.XMR = obj.XMR + btc
                    obj.save()
                else:
                    print('error')
                    flag = 'buy'
            elif request.POST.get("sell"):
                btc = form_data.cleaned_data['XMR']
                if btc <= obj.XMR:
                    obj.KRW = int(obj.KRW) + int(float(price['price_krw']) * float(btc))
                    obj.XMR = obj.XMR - btc
                    obj.save()
                else:
                    print('error')
                    flag = 'sell'


    context = {'price':price,'form': form_data, 'wallet': obj, 'is_login': request.user.is_authenticated,'flag':flag}
    context.update(csrf(request))
    return HttpResponse(template.render(context))

def market_btg(request):
    status = get_info('bitcoin-gold')
    price = status[0][0]
    price['24h_volume_krw'] = format(int(float(price['24h_volume_krw'])),',')
    price['price_krw'] = int(float(price['price_krw']))
    price['market_cap_krw'] = format(int(float(price['market_cap_krw'])), ',')
    if (price['max_supply']):
        price['max_supply'] = format(int(float(price['max_supply'])), ',')
    price['total_supply'] = format(int(float(price['total_supply'])), ',')
    pk = request.POST.get('pk', None)
    template = get_template('btg.html')
    form_data = btg_form()
    obj = wallet.objects.get(Owner=request.user)
    flag = 'none'

    if request.is_ajax():
        data = price['price_krw'] * float(request.GET['Amount'])
        import json
        return HttpResponse(json.dumps({'message':data}), 'application/json')
    if request.method == 'POST':
        form_data = btg_form(request.POST)
        if form_data.is_valid():
            if request.POST.get("buy"):
                btc = form_data.cleaned_data['BTG']

                if price['price_krw'] * btc < obj.KRW:
                    obj.KRW = int(obj.KRW) - int(float(price['price_krw']) * float(btc))
                    obj.BTG = obj.BTG + btc
                    obj.save()
                else:
                    print('error')
                    flag = 'buy'
            elif request.POST.get("sell"):
                btc = form_data.cleaned_data['BTG']
                if btc <= obj.BTG:
                    obj.KRW = int(obj.KRW) + int(float(price['price_krw']) * float(btc))
                    obj.BTG = obj.BTG - btc
                    obj.save()
                else:
                    print('error')
                    flag = 'sell'


    context = {'price':price,'form': form_data, 'wallet': obj, 'is_login': request.user.is_authenticated,'flag':flag}
    context.update(csrf(request))
    return HttpResponse(template.render(context))

def market_eos(request):
    status = get_info('eos')
    price = status[0][0]
    price['24h_volume_krw'] = format(int(float(price['24h_volume_krw'])),',')
    price['price_krw'] = int(float(price['price_krw']))
    price['market_cap_krw'] = format(int(float(price['market_cap_krw'])), ',')
    if (price['max_supply']):
        price['max_supply'] = format(int(float(price['max_supply'])), ',')
    price['total_supply'] = format(int(float(price['total_supply'])), ',')
    pk = request.POST.get('pk', None)
    template = get_template('eos.html')
    form_data = eos_form()
    obj = wallet.objects.get(Owner=request.user)
    flag = 'none'

    if request.is_ajax():
        data = price['price_krw'] * float(request.GET['Amount'])
        import json
        return HttpResponse(json.dumps({'message':data}), 'application/json')
    if request.method == 'POST':
        form_data = eos_form(request.POST)
        if form_data.is_valid():
            if request.POST.get("buy"):
                btc = form_data.cleaned_data['EOS']

                if price['price_krw'] * btc < obj.KRW:
                    obj.KRW = int(obj.KRW) - int(float(price['price_krw']) * float(btc))
                    obj.EOS = obj.EOS + btc
                    obj.save()
                else:
                    print('error')
                    flag = 'buy'
            elif request.POST.get("sell"):
                btc = form_data.cleaned_data['EOS']
                if btc <= obj.EOS:
                    obj.KRW = int(obj.KRW) + int(float(price['price_krw']) * float(btc))
                    obj.EOS = obj.EOS - btc
                    obj.save()
                else:
                    print('error')
                    flag = 'sell'


    context = {'price':price,'form': form_data, 'wallet': obj, 'is_login': request.user.is_authenticated,'flag':flag}
    context.update(csrf(request))
    return HttpResponse(template.render(context))

def market_qtum(request):
    status = get_info('qtum')
    price = status[0][0]
    price['24h_volume_krw'] = format(int(float(price['24h_volume_krw'])),',')
    price['price_krw'] = int(float(price['price_krw']))
    price['market_cap_krw'] = format(int(float(price['market_cap_krw'])), ',')
    if (price['max_supply']):
        price['max_supply'] = format(int(float(price['max_supply'])), ',')
    price['total_supply'] = format(int(float(price['total_supply'])), ',')
    pk = request.POST.get('pk', None)
    template = get_template('qtum.html')
    form_data = qtum_form()
    obj = wallet.objects.get(Owner=request.user)
    flag = 'none'

    if request.is_ajax():
        data = price['price_krw'] * float(request.GET['Amount'])
        import json
        return HttpResponse(json.dumps({'message':data}), 'application/json')
    if request.method == 'POST':
        form_data = qtum_form(request.POST)
        if form_data.is_valid():
            if request.POST.get("buy"):
                btc = form_data.cleaned_data['QTUM']

                if price['price_krw'] * btc < obj.KRW:
                    obj.KRW = int(obj.KRW) - int(float(price['price_krw']) * float(btc))
                    obj.QTUM = obj.QTUM + btc
                    obj.save()
                else:
                    print('error')
                    flag = 'buy'
            elif request.POST.get("sell"):
                btc = form_data.cleaned_data['QTUM']
                if btc <= obj.QTUM:
                    obj.KRW = int(obj.KRW) + int(float(price['price_krw']) * float(btc))
                    obj.QTUM = obj.QTUM - btc
                    obj.save()
                else:
                    print('error')
                    flag = 'sell'


    context = {'price':price,'form': form_data, 'wallet': obj, 'is_login': request.user.is_authenticated,'flag':flag}
    context.update(csrf(request))
    return HttpResponse(template.render(context))

def market_etc(request):
    status = get_info('ethereum-classic')
    price = status[0][0]
    price['24h_volume_krw'] = format(int(float(price['24h_volume_krw'])),',')
    price['price_krw'] = int(float(price['price_krw']))
    price['market_cap_krw'] = format(int(float(price['market_cap_krw'])), ',')
    if (price['max_supply']):
        price['max_supply'] = format(int(float(price['max_supply'])), ',')
    price['total_supply'] = format(int(float(price['total_supply'])), ',')
    pk = request.POST.get('pk', None)
    template = get_template('etc.html')
    form_data = etc_form()
    obj = wallet.objects.get(Owner=request.user)
    flag = 'none'

    if request.is_ajax():
        data = price['price_krw'] * float(request.GET['Amount'])
        import json
        return HttpResponse(json.dumps({'message':data}), 'application/json')
    if request.method == 'POST':
        form_data = etc_form(request.POST)
        if form_data.is_valid():
            if request.POST.get("buy"):
                btc = form_data.cleaned_data['ETC']

                if price['price_krw'] * btc < obj.KRW:
                    obj.KRW = int(obj.KRW) - int(float(price['price_krw']) * float(btc))
                    obj.ETC = obj.ETC + btc
                    obj.save()
                else:
                    print('error')
                    flag = 'buy'
            elif request.POST.get("sell"):
                btc = form_data.cleaned_data['ETC']
                if btc <= obj.ETC:
                    obj.KRW = int(obj.KRW) + int(float(price['price_krw']) * float(btc))
                    obj.ETC = obj.ETC - btc
                    obj.save()
                else:
                    print('error')
                    flag = 'sell'


    context = {'price':price,'form': form_data, 'wallet': obj, 'is_login': request.user.is_authenticated,'flag':flag}
    context.update(csrf(request))
    return HttpResponse(template.render(context))

def market_zec(request):
    status = get_info('zcash')
    price = status[0][0]
    price['24h_volume_krw'] = format(int(float(price['24h_volume_krw'])),',')
    price['price_krw'] = int(float(price['price_krw']))
    price['market_cap_krw'] = format(int(float(price['market_cap_krw'])), ',')
    if (price['max_supply']):
        price['max_supply'] = format(int(float(price['max_supply'])), ',')
    price['total_supply'] = format(int(float(price['total_supply'])), ',')
    pk = request.POST.get('pk', None)
    template = get_template('zec.html')
    form_data = zec_form()
    obj = wallet.objects.get(Owner=request.user)
    flag = 'none'

    if request.is_ajax():
        data = price['price_krw'] * float(request.GET['Amount'])
        import json
        return HttpResponse(json.dumps({'message':data}), 'application/json')
    if request.method == 'POST':
        form_data = zec_form(request.POST)
        if form_data.is_valid():
            if request.POST.get("buy"):
                btc = form_data.cleaned_data['ZEC']

                if price['price_krw'] * btc < obj.KRW:
                    obj.KRW = int(obj.KRW) - int(float(price['price_krw']) * float(btc))
                    obj.ZEC = obj.ZEC + btc
                    obj.save()
                else:
                    print('error')
                    flag = 'buy'
            elif request.POST.get("sell"):
                btc = form_data.cleaned_data['ZEC']
                if btc <= obj.ZEC:
                    obj.KRW = int(obj.KRW) + int(float(price['price_krw']) * float(btc))
                    obj.ZEC = obj.ZEC - btc
                    obj.save()
                else:
                    print('error')
                    flag = 'sell'


    context = {'price':price,'form': form_data, 'wallet': obj, 'is_login': request.user.is_authenticated,'flag':flag}
    context.update(csrf(request))
    return HttpResponse(template.render(context))

def market_pixel(request):
    template = get_template('pixelmarket.html')
    form_data = pixel_form()
    obj = pixel_table.objects.get(Owner=request.user)
    wal = wallet.objects.get(Owner=request.user)
    flag = 'none'

    if request.method == 'POST':
        form_data = pixel_form(request.POST)
        if form_data.is_valid():
            x = form_data.cleaned_data['x']
            y = form_data.cleaned_data['y']
            w = form_data.cleaned_data['w']
            h = form_data.cleaned_data['h']

            if int(w[:-1])*int(h[:-1])*100000 < wal.KRW:
                obj.x = x
                obj.y = y
                obj.width = w
                obj.height = h
                obj.reg = datetime.datetime.now()
                obj.save()
                wal.KRW = int(wal.KRW) - (int(w[:-1])*int(h[:-1])*100000)
                wal.save()
                return redirect('/market/main')
            else:
                print('error')
                flag = 'buy'


    context = {'form': form_data, 'pix': obj, 'is_login': request.user.is_authenticated,'flag':flag}
    context.update(csrf(request))
    return HttpResponse(template.render(context))