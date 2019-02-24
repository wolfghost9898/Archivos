# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.deletion
import datetime


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0013_auto_20181026_2021'),
    ]

    operations = [
        migrations.CreateModel(
            name='Estadio',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('Nombre', models.CharField(max_length=100)),
                ('Fecha', models.DateTimeField()),
                ('cantidad', models.IntegerField()),
                ('Direccion', models.CharField(max_length=100)),
                ('Estado', models.CharField(max_length=100)),
                ('Foto', models.ImageField(upload_to=b'', blank=True)),
                ('Pais', models.ForeignKey(related_name='PaisEstadio', on_delete=django.db.models.deletion.PROTECT, to='Web.Pais')),
            ],
            options={
                'db_table': 'Estadio',
            },
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='key_expires',
            field=models.DateTimeField(default=datetime.date(2018, 10, 27)),
        ),
    ]
