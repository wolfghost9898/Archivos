# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0024_auto_20181029_2212'),
    ]

    operations = [
        migrations.AlterField(
            model_name='partido',
            name='Asistencia',
            field=models.IntegerField(default=0),
        ),
        migrations.AlterField(
            model_name='partido',
            name='Resultado',
            field=models.CharField(default=b'0-0', max_length=100),
        ),
    ]
