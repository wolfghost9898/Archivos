# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import datetime


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0008_auto_20181021_1956'),
    ]

    operations = [
        migrations.AlterField(
            model_name='userprofile',
            name='key_expires',
            field=models.DateTimeField(default=datetime.date(2018, 10, 23)),
        ),
        migrations.AlterField(
            model_name='usuario',
            name='Imagen',
            field=models.ImageField(upload_to=b'', blank=True),
        ),
    ]
