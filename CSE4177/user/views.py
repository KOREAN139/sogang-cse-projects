from django.contrib import auth
from django.contrib.auth.models import User
from django.shortcuts import render, redirect
from django.http.response import HttpResponse
from django.template.loader import get_template
from user.forms import LoginForm, JoinForm
from django.template.context_processors import csrf
from django.contrib.auth import logout
from user.models import *

# Create your views here.


def login(request):
    template = get_template('login.html')
    if request.method == 'POST':
        temp = request.POST
    context = {'login_form': LoginForm(), 'is_login': request.user.is_authenticated}
    context.update(csrf(request))
    return HttpResponse(template.render(context))


def login_validate(request):
    login_form_data = LoginForm(request.POST)
    if request.method == 'POST':
        temp = request.POST

    if login_form_data.is_valid():
        user = auth.authenticate(request, username=login_form_data.cleaned_data['email'],
                                 password=login_form_data.cleaned_data['password'])
        if user is not None:
            if user.is_active:
                auth.login(request, user)

                return redirect('/market/main')
        else:
            logout(request)
            return redirect('/user/login')

    return HttpResponse('unknown error')

def about(request):
    template = get_template('about.html')
    context = {}
    context.update(csrf(request))
    return HttpResponse(template.render(context))

def join_page(request):
    if request.method == 'POST':
        form_data = JoinForm(request.POST)

        if form_data.is_valid():
            pass1 = form_data.cleaned_data['password']
            pass2 = form_data.cleaned_data['password_check']
            color = form_data.cleaned_data['color']
            if pass1 == pass2:
                username = form_data.cleaned_data['email']
                password = form_data.cleaned_data['password']
                User.objects.create_user(username=username, password=password)
                user = auth.authenticate(request, username=username,password=password)
                auth.login(request, user)
                pixel_table.objects.create(x='0%',y='0%',width='0%',height='0%',Owner=username,color=color)

                return redirect('/market/new/')
            else:
                return redirect('/user/join/')
        else:
            return redirect('/user/join/')
    else:
        form_data = JoinForm()

    template = get_template('join.html')
    context = {'join_form': form_data, 'is_login': request.user.is_authenticated}
    context.update(csrf(request))
    return HttpResponse(template.render(context))




# Create your views here.
