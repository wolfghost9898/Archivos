# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0014_auto_20181027_1716'),
    ]

    operations = [
        migrations.CreateModel(
            name='Jugador',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('Nombre', models.CharField(max_length=100)),
                ('Fecha_nac', models.DateTimeField()),
                ('Fecha_inicio', models.DateTimeField()),
                ('Fecha_fin', models.DateTimeField()),
                ('Estado', models.CharField(max_length=100)),
                ('Posicion', models.CharField(max_length=100)),
                ('Equipo', models.ForeignKey(related_name='EquipoJugador', on_delete=django.db.models.deletion.PROTECT, to='Web.Equipo')),
                ('Pais', models.ForeignKey(related_name='PaisJugador', on_delete=django.db.models.deletion.PROTECT, to='Web.Pais')),
            ],
            options={
                'db_table': 'Jugador',
            },
        ),
    ]
