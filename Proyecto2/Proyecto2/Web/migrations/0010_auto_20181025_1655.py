# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import datetime


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0009_auto_20181023_1900'),
    ]

    operations = [
        migrations.CreateModel(
            name='Historial',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('Descripcion', models.CharField(max_length=100)),
                ('Accion', models.IntegerField()),
                ('Admin', models.ForeignKey(related_name='Admin_accion', to='Web.Usuario')),
                ('Usuario', models.ForeignKey(related_name='Usuario_afectado', to='Web.Usuario')),
            ],
            options={
                'db_table': 'Historial',
            },
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='key_expires',
            field=models.DateTimeField(default=datetime.date(2018, 10, 25)),
        ),
    ]
