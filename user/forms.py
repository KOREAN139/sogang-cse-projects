from django import forms

class LoginForm(forms.Form):
    email = forms.CharField(label="EMAIL",max_length=30,widget=(forms.EmailInput(attrs={'class':'text-field w-input','placeholder':'E-Mail'})))
    password = forms.CharField(label="PASSWORD" ,max_length=12,widget=forms.PasswordInput(attrs={'class':'text-field-2 w-input','placeholder':'Password'}))

class JoinForm(forms.Form):
    email = forms.CharField(label="EMAIL",max_length=30,widget=(forms.EmailInput(attrs={'class':'text-field w-input','placeholder':'E-Mail'})))
    password = forms.CharField(label="PASSWORD" ,max_length=12,widget=forms.PasswordInput(attrs={'class':'text-field-2 w-input','placeholder':'Password'}))
    password_check = forms.CharField(label="PASSWORD(again)",min_length=6,max_length=12,required=True,
                                     widget=forms.PasswordInput(attrs={'class':'text-field-2 w-input','placeholder':'Password_Check'}))
    color = forms.CharField(label="COLOR", max_length=30,
                            widget=(forms.TextInput(attrs={'type': 'color','class': 'w-input'})))
