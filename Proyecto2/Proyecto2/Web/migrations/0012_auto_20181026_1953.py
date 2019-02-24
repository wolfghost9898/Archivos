# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import datetime


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0011_pais'),
    ]

    operations = [
        migrations.CreateModel(
            name='Equipo',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('Nombre', models.CharField(max_length=100)),
                ('Fecha', models.IntegerField()),
                ('Logo', models.ImageField(upload_to=b'', blank=True)),
                ('Pais', models.ForeignKey(related_name='Pais', to='Web.Pais')),
            ],
            options={
                'db_table': 'Equipo',
            },
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='key_expires',
            field=models.DateTimeField(default=datetime.date(2018, 10, 26)),
        ),
    ]
