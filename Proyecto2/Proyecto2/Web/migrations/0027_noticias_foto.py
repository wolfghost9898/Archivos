# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0026_auto_20181030_1022'),
    ]

    operations = [
        migrations.AddField(
            model_name='noticias',
            name='Foto',
            field=models.ImageField(upload_to=b'', blank=True),
        ),
    ]
