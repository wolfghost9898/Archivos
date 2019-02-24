# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import datetime


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0023_auto_20181029_2207'),
    ]

    operations = [
        migrations.AlterField(
            model_name='estadio',
            name='Fecha',
            field=models.DateTimeField(default=datetime.date.today),
        ),
    ]
