# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0002_auto_20181007_2251'),
    ]

    operations = [
        migrations.AlterField(
            model_name='usuario',
            name='Imagen',
            field=models.ImageField(default=b'default.png', upload_to=b''),
        ),
    ]
