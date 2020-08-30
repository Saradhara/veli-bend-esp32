# Generated by Django 2.2.12 on 2020-08-27 19:05

from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='TraceEvent',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('partner_uuid', models.CharField(max_length=36)),
                ('self_uuid', models.CharField(max_length=36)),
                ('distance', models.IntegerField()),
                ('duration', models.IntegerField()),
                ('covid_risk', models.CharField(choices=[('HIGH', 'HIGH'), ('MEDIUM', 'MEDIUM')], max_length=8)),
                ('date', models.DateTimeField(default=None)),
                ('issue', models.CharField(default=None, max_length=60)),
                ('created_at', models.DateTimeField(auto_now_add=True)),
                ('updated_at', models.DateTimeField(auto_now=True)),
            ],
        ),
    ]