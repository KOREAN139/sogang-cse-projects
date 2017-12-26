from django import forms
from decimal import Decimal
from PixcelCoin import settings
from market.models import *
import datetime
from user.models import *
class btc_form(forms.ModelForm):
    BTC = forms.DecimalField(label="BTC",max_digits=20,decimal_places=8,required=False,initial=0,
                             widget=forms.NumberInput(attrs={'class':'text-field-3 w-input','id':'Amount'}))
    class Meta:
        model=wallet
        fields = ['BTC']

class eth_form(forms.ModelForm):
    ETH = forms.DecimalField(label="ETH", max_digits=20, decimal_places=8, required=False, initial=0,
                             widget=forms.NumberInput(attrs={'class': 'text-field-3 w-input', 'id': 'Amount'}))
    class Meta:
        model=wallet
        fields = ['ETH']

class bch_form(forms.ModelForm):
    BCH = forms.DecimalField(label="BCH", max_digits=20, decimal_places=8, required=False, initial=0,
                             widget=forms.NumberInput(attrs={'class': 'text-field-3 w-input', 'id': 'Amount'}))
    class Meta:
        model=wallet
        fields = ['BCH']

class xrp_form(forms.ModelForm):
    XRP = forms.DecimalField(label="XRP", max_digits=20, decimal_places=8, required=False, initial=0,
                             widget=forms.NumberInput(attrs={'class': 'text-field-3 w-input', 'id': 'Amount'}))
    class Meta:
        model=wallet
        fields = ['XRP']

class ltc_form(forms.ModelForm):
    LTC = forms.DecimalField(label="LTC", max_digits=20, decimal_places=8, required=False, initial=0,
                             widget=forms.NumberInput(attrs={'class': 'text-field-3 w-input', 'id': 'Amount'}))
    class Meta:
        model=wallet
        fields = ['LTC']

class dash_form(forms.ModelForm):
    DASH = forms.DecimalField(label="DASH", max_digits=20, decimal_places=8, required=False, initial=0,
                              widget=forms.NumberInput(attrs={'class': 'text-field-3 w-input', 'id': 'Amount'}))
    class Meta:
        model=wallet
        fields = ['DASH']

class xmr_form(forms.ModelForm):
    XMR = forms.DecimalField(label="XMR", max_digits=20, decimal_places=8, required=False, initial=0,
                             widget=forms.NumberInput(attrs={'class': 'text-field-3 w-input', 'id': 'Amount'}))
    class Meta:
        model=wallet
        fields = ['XMR']

class btg_form(forms.ModelForm):
    BTG = forms.DecimalField(label="BTG", max_digits=20, decimal_places=8, required=False, initial=0,
                             widget=forms.NumberInput(attrs={'class': 'text-field-3 w-input', 'id': 'Amount'}))
    class Meta:
        model = wallet
        fields = ['BTG']

class eos_form(forms.ModelForm):
    XMR = forms.DecimalField(label="EOS", max_digits=20, decimal_places=8, required=False, initial=0,
                             widget=forms.NumberInput(attrs={'class': 'text-field-3 w-input', 'id': 'Amount'}))

    class Meta:
        model = wallet
        fields = ['EOS']

class qtum_form(forms.ModelForm):
    BTG = forms.DecimalField(label="QTUM", max_digits=20, decimal_places=8, required=False, initial=0,
                             widget=forms.NumberInput(attrs={'class': 'text-field-3 w-input', 'id': 'Amount'}))

    class Meta:
        model = wallet
        fields = ['QTUM']

class etc_form(forms.ModelForm):
    BTG = forms.DecimalField(label="ETC", max_digits=20, decimal_places=8, required=False, initial=0,
                             widget=forms.NumberInput(attrs={'class': 'text-field-3 w-input', 'id': 'Amount'}))

    class Meta:
        model = wallet
        fields = ['ETC']

class zec_form(forms.ModelForm):
    XMR = forms.DecimalField(label="ZEC", max_digits=20, decimal_places=8, required=False, initial=0,
                             widget=forms.NumberInput(attrs={'class':'text-field-3 w-input','id':'Amount'}))

    class Meta:
        model = wallet
        fields = ['ZEC']


    #   class Meta:
    #       model=tblDemograph
 #       fields=['dbID','NoDemo','DoB','Sex','Marital_Status','Rehab_Setting',]

class pixel_form(forms.ModelForm):
    x = forms.CharField(label="X", max_length=30)
    y = forms.CharField(label="Y", max_length=30)
    w = forms.CharField(label="W", max_length=30)
    h = forms.CharField(label="H", max_length=30)



    class Meta:
        model = pixel_table
        fields = ['x','y','w','h']


    #   class Meta:
    #       model=tblDemograph
 #       fields=['dbID','NoDemo','DoB','Sex','Marital_Status','Rehab_Setting',]
"""
    KRW = models.DecimalField(max_digits=20, decimal_places=0)
    BTC = models.DecimalField(max_digits=20, decimal_places=8)
    ETH = models.DecimalField(max_digits=20, decimal_places=8)
    BCH = models.DecimalField(max_digits=20, decimal_places=8)
    XRP = models.DecimalField(max_digits=20, decimal_places=8)
    LTC = models.DecimalField(max_digits=20, decimal_places=8)
    DASH = models.DecimalField(max_digits=20, decimal_places=8)
    XMR = models.DecimalField(max_digits=20, decimal_places=8)
    BTG = models.DecimalField(max_digits=20, decimal_places=8)
    EOS = models.DecimalField(max_digits=20, decimal_places=8)
    QTUM = models.DecimalField(max_digits=20, decimal_places=8)
    ETC = models.DecimalField(max_digits=20, decimal_places=8)
    ZEC = models.DecimalField(max_digits=20, decimal_places=8)

    class Meta:
        ordering = ['KRW', ]
"""