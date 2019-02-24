# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.deletion
import datetime


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0019_auto_20181028_1828'),
    ]

    operations = [
        migrations.CreateModel(
            name='Partido',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('Fecha', models.DateTimeField()),
                ('Asistencia', models.IntegerField()),
                ('Resultado', models.CharField(max_length=100)),
                ('Estado', models.CharField(max_length=100)),
                ('EquipoLocal', models.ForeignKey(related_name='EquipoLocal', on_delete=django.db.models.deletion.PROTECT, to='Web.Equipo')),
                ('EquipoVisita', models.ForeignKey(related_name='EquipoVisita', on_delete=django.db.models.deletion.PROTECT, to='Web.Equipo')),
                ('Estadio', models.ForeignKey(related_name='EstadioPais', on_delete=django.db.models.deletion.PROTECT, to='Web.Estadio')),
            ],
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='key_expires',
            field=models.DateTimeField(default=datetime.date(2018, 10, 29)),
        ),
    ]
