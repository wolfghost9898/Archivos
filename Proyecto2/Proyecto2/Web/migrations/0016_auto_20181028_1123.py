# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.deletion
import datetime


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0015_jugador'),
    ]

    operations = [
        migrations.CreateModel(
            name='BitacoraJugador',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('Fecha_inicio', models.DateTimeField()),
                ('Fecha_fin', models.DateTimeField()),
                ('Equipo', models.ForeignKey(related_name='EquipoJugadorBitacora', on_delete=django.db.models.deletion.PROTECT, to='Web.Equipo')),
                ('Jugador', models.ForeignKey(related_name='JugadorBitacora', on_delete=django.db.models.deletion.PROTECT, to='Web.Jugador')),
            ],
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='key_expires',
            field=models.DateTimeField(default=datetime.date(2018, 10, 28)),
        ),
    ]
