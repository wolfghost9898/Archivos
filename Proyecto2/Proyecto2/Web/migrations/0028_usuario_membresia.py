# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0027_noticias_foto'),
    ]

    operations = [
        migrations.AddField(
            model_name='usuario',
            name='Membresia',
            field=models.IntegerField(default=0),
        ),
    ]
