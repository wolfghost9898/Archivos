# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0017_auto_20181028_1452'),
    ]

    operations = [
        migrations.CreateModel(
            name='Competencia',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('Nombre', models.CharField(max_length=100)),
                ('Anio', models.IntegerField()),
                ('Tipo', models.CharField(max_length=100)),
                ('Campeon', models.ForeignKey(related_name='CampeonCompetencia', on_delete=django.db.models.deletion.PROTECT, to='Web.Equipo')),
                ('Pais', models.ForeignKey(related_name='PaisCompetencia', on_delete=django.db.models.deletion.PROTECT, to='Web.Pais')),
            ],
            options={
                'db_table': 'Competencia',
            },
        ),
        migrations.CreateModel(
            name='Historial_Competencia',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('Competencia', models.ForeignKey(related_name='CompetenciaHistorial', on_delete=django.db.models.deletion.PROTECT, to='Web.Competencia')),
                ('Equipo', models.ForeignKey(related_name='EquipoHistorial', on_delete=django.db.models.deletion.PROTECT, to='Web.Equipo')),
            ],
            options={
                'db_table': 'Historial_Competencia',
            },
        ),
    ]
