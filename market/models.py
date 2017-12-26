from django.db import models
from django_mysql.models import ListCharField
from decimal import Decimal
from django.contrib.auth.models import User
from multiselectfield import MultiSelectField,MultiSelectFormField
# Create your models here.

class wallet(models.Model):
    Owner = models.CharField(max_length=30,primary_key=True,unique=True)
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


