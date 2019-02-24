# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0007_auto_20181021_1659'),
    ]

    operations = [
        migrations.AlterField(
            model_name='usuario',
            name='Fecha_nac',
            field=models.DateTimeField(),
        ),
    ]
