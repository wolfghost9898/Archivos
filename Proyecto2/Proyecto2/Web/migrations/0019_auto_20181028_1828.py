# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0018_competencia_historial_competencia'),
    ]

    operations = [
        migrations.AlterField(
            model_name='competencia',
            name='Campeon',
            field=models.ForeignKey(related_name='CampeonCompetencia', on_delete=django.db.models.deletion.PROTECT, to='Web.Equipo', null=True),
        ),
    ]
