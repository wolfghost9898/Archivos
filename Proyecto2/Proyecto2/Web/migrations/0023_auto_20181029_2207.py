# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import datetime


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0022_incidencias'),
    ]

    operations = [
        migrations.AlterField(
            model_name='jugador',
            name='Fecha_fin',
            field=models.DateTimeField(default=datetime.date.today),
        ),
        migrations.AlterField(
            model_name='jugador',
            name='Fecha_inicio',
            field=models.DateTimeField(default=datetime.date.today),
        ),
        migrations.AlterField(
            model_name='jugador',
            name='Fecha_nac',
            field=models.DateTimeField(default=datetime.date.today),
        ),
    ]
