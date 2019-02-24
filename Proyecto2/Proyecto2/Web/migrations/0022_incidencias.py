# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0021_partido_competencia'),
    ]

    operations = [
        migrations.CreateModel(
            name='Incidencias',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('Minuto', models.IntegerField()),
                ('Descripcion', models.CharField(max_length=100)),
                ('Equipo', models.ForeignKey(related_name='IncidenciaEquipo', on_delete=django.db.models.deletion.PROTECT, to='Web.Equipo')),
                ('Jugador', models.ForeignKey(related_name='JugadorIncidencia', on_delete=django.db.models.deletion.PROTECT, to='Web.Jugador')),
                ('Partido', models.ForeignKey(related_name='IncidenciaPartido', on_delete=django.db.models.deletion.PROTECT, to='Web.Partido')),
            ],
            options={
                'db_table': 'Incidencia',
            },
        ),
    ]
