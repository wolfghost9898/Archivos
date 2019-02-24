# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0029_membresia'),
    ]

    operations = [
        migrations.RenameModel(
            old_name='Membresia',
            new_name='Membre',
        ),
        migrations.AlterModelTable(
            name='membre',
            table='Membresia',
        ),
    ]
