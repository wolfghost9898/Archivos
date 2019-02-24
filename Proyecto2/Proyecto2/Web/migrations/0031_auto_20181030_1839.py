# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0030_auto_20181030_1837'),
    ]

    operations = [
        migrations.AlterField(
            model_name='membre',
            name='Fecha_fin',
            field=models.DateTimeField(),
        ),
    ]
