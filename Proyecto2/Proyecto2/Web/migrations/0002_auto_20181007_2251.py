# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0001_initial'),
    ]

    operations = [
        migrations.AlterField(
            model_name='usuario',
            name='Tipo',
            field=models.IntegerField(default=1),
        ),
        migrations.AlterModelTable(
            name='usuario',
            table='Usuario',
        ),
    ]
