from django.db import models

class pixel_table(models.Model):
    Owner = models.CharField(max_length=30,primary_key=True,unique=True)
    x = models.CharField(max_length=30)
    y = models.CharField(max_length=30)
    width = models.CharField(max_length=30)
    height = models.CharField(max_length=30)
    color = models.CharField(max_length=30)
    reg = models.DateTimeField(auto_now=True)
    class Meta:
        ordering = ['reg', ]
# Create your models here.
