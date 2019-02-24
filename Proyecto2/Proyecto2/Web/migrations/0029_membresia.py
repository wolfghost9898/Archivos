# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0028_usuario_membresia'),
    ]

    operations = [
        migrations.CreateModel(
            name='Membresia',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('Fecha_inicio', models.DateTimeField(auto_now_add=True)),
                ('Fecha_fin', models.DateTimeField(auto_now_add=True)),
                ('Usuario', models.ForeignKey(related_name='MembresiaUsuario', on_delete=django.db.models.deletion.PROTECT, to='Web.Usuario')),
            ],
        ),
    ]
