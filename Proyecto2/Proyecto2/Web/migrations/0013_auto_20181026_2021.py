# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0012_auto_20181026_1953'),
    ]

    operations = [
        migrations.AlterField(
            model_name='equipo',
            name='Pais',
            field=models.ForeignKey(related_name='Pais', on_delete=django.db.models.deletion.PROTECT, to='Web.Pais'),
        ),
    ]
