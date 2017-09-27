<?php
/* Schema For Data Premis */
$schema['zdatanegeri'] = array(
    'description' => 'Stores information about data premis.',
    'fields' => array(
        'id' => array(
            'type' => 'varchar',
            'length' => 30,
            'not null' => TRUE,
            'default' => 'random_number',
            'description' => 'Negeri ID dari data negeri',
        ),
        'zone' => array(
            'type' => 'varchar',
            'length' => 3,
            'not null' => TRUE,
            'default' => 'Zon',
            'description' => 'Kode zon dari data negeri',
        ),
        'nama' => array(
            'type' => 'varchar',
            'length' => 500,
            'not null' => TRUE,
            'default' => 'Nama Negeri',
            'description' => 'Nama negeri dari data negeri',
        ),
        'alamat' => array(
            'type' => 'varchar',
            'length' => 1000,
            'not null' => TRUE,
            'default' => 'Alamat Pejabat',
            'description' => 'Alamat pejabat pentadbiran dari data negeri',
        ),
        'telepon' => array(
            'type' => 'varchar',
            'length' => 120,
            'not null' => TRUE,
            'default' => '0000000000',
            'description' => 'Telepon pejabat dari data negeri',
        ),
        'email' => array(
            'type' => 'varchar',
            'length' => 255,
            'not null' => TRUE,
            'default' => 'admin@ikhwanit.com',
            'description' => 'Email negeri dari data negeri',
        ),
        'website' => array(
            'type' => 'varchar',
            'length' => 255,
            'not null' => TRUE,
            'default' => 'www.ikhwanit.com',
            'description' => 'Website negeri dari data negeri',
        ),
        'created' => array(
            'type' => 'int',
            'default' => 0,
            'description' => 'The time that the row was created, as a Unix timestamp.',
        ),
        'changed' => array(
            'type' => 'int',
            'default' => 0,
            'description' => 'The time that the row was last edited, as a Unix timestamp.',
        ),
        'uid' => array(
            'type' => 'int',
            'default' => 0,
            'description' => 'Uid that created the record.',
        ),
        'uid_changed' => array(
            'type' => 'int',
            'default' => 0,
            'description' => 'Uid that changing the record.',
        ),
    ),
    'primary key' => array('id'),
    'indexes' => array(
        'zone'          => array('zone'),
        'nama'          => array('nama'),
        'alamat'        => array('alamat'),
        'telepon'       => array('telepon'),
        'uid_changed'   => array('uid_changed'),
        'created'       => array('created'),
        'changed'       => array('changed'),
    ),
    'foreign keys' => array(
        'zone' => array(
            'table' => 'zdatazone',
            'columns' => array('zone' => 'zone'),
        ),
    ),
);