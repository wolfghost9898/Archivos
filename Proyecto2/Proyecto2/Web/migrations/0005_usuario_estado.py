# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0004_auto_20181008_1916'),
    ]

    operations = [
        migrations.AddField(
            model_name='usuario',
            name='Estado',
            field=models.IntegerField(default=0),
        ),
    ]
