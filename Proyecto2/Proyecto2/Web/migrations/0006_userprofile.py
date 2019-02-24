# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import datetime


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0005_usuario_estado'),
    ]

    operations = [
        migrations.CreateModel(
            name='UserProfile',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('activation_key', models.CharField(max_length=40, blank=True)),
                ('key_expires', models.DateTimeField(default=datetime.date(2018, 10, 10))),
                ('user', models.OneToOneField(to='Web.Usuario')),
            ],
            options={
                'verbose_name_plural': 'Perfiles de Usuario',
            },
        ),
    ]
