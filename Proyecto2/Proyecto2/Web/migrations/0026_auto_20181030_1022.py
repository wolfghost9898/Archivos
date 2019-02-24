# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.deletion
import datetime


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0025_auto_20181029_2220'),
    ]

    operations = [
        migrations.CreateModel(
            name='Noticias',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('Titulo', models.CharField(max_length=100)),
                ('Cuerpo', models.CharField(max_length=600)),
                ('Fecha', models.DateTimeField(auto_now_add=True)),
                ('Empleado', models.ForeignKey(related_name='EmpleadoNoticias', on_delete=django.db.models.deletion.PROTECT, to='Web.Usuario')),
                ('Equipo', models.ForeignKey(related_name='NoticiaEquipo', on_delete=django.db.models.deletion.PROTECT, to='Web.Equipo')),
            ],
            options={
                'db_table': 'Noticias',
            },
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='key_expires',
            field=models.DateTimeField(default=datetime.date(2018, 10, 30)),
        ),
    ]
