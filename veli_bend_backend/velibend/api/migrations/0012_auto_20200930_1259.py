# Generated by Django 2.2.12 on 2020-09-30 07:29

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('api', '0011_auto_20200830_1656'),
    ]

    operations = [
        migrations.AlterField(
            model_name='traceevent',
            name='partner_uuid',
            field=models.CharField(max_length=36),
        ),
        migrations.AlterField(
            model_name='traceevent',
            name='self_uuid',
            field=models.CharField(max_length=36),
        ),
    ]