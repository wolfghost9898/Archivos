# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0020_auto_20181029_1628'),
    ]

    operations = [
        migrations.AddField(
            model_name='partido',
            name='Competencia',
            field=models.ForeignKey(related_name='CompetenciaPartido', on_delete=django.db.models.deletion.PROTECT, default=2, to='Web.Competencia'),
            preserve_default=False,
        ),
    ]
