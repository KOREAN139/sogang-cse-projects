# Generated by Django 2.0 on 2017-12-25 23:30

from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='pixcel_table',
            fields=[
                ('Owner', models.CharField(max_length=30, primary_key=True, serialize=False, unique=True)),
                ('x', models.CharField(max_length=30)),
                ('y', models.CharField(max_length=30)),
                ('width', models.CharField(max_length=30)),
                ('height', models.CharField(max_length=30)),
                ('color', models.CharField(max_length=30)),
            ],
        ),
    ]
