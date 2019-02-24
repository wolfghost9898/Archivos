# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0003_auto_20181008_1911'),
    ]

    operations = [
        migrations.AlterField(
            model_name='usuario',
            name='Imagen',
            field=models.ImageField(upload_to=b'media', blank=True),
        ),
    ]
