<?php

function array_jenis_kelamin(){
    return array(t('Laki-laki'), t('Perempuan'));
}
function list_staff(){
    $core_module_path = drupal_get_path('module','ikhwan_core');
    $csspath = $core_module_path.'/css/custom-style.css';
    drupal_add_css($csspath);
    _add_jeditable_plugins();
    $module_path  = drupal_get_path('module','data_staff');
    $jspath = $module_path.'/js/list_staff.js';
    drupal_add_js($jspath,array('scope' => 'footer', 'weight' => 5));
    // Table settings.
    $attributes['datatable_options'] = array(
        'bFilter'   => TRUE,
        'bInfo'     => TRUE,
        'bStateSave' => TRUE,
        'sPaginationType' => 'full_numbers',
    );
    // Define table columns
    $header = array(
        array(
            'data' => t('Nama Staff'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Nama Lengkap'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Nama Hijrah'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('JK'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('No. KTP'),
            'class' => array('align-center'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('No. KK'),
            'class' => array('align-center'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('No. HP'),
            'class' => array('align-center'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Alamat'),
            'class' => array('col-input-number align-center'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Tgl. Lahir'),
            'class' => array('col-input-number align-center'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Tempat Lahir'),
            'datatable_options' => array(
                'bSortable' => FALSE,
                'bSearchable' => FALSE,
            ),
        ),
        array(
            'data' => t('Umur'),
            'class' => array('col-input-number-small align-center'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Suami/Istri'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Ayah'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Premis'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Negeri'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Zone'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
    );
    $keuanganZone = cek_keuangan_zone();
    $userZone = null;
    if ($keuanganZone) {
        $userZone = get_user_zone();
    }
    if ($userZone[0]){
        $getDataZone = $userZone[1]['zone'];
        $dataPremis = $userZone[1]['id_premis'];
    }else{
        $getDataZone = getZoneByCode();
        $dataPremis = get_data_premis_by_id();
    }
    $namaZone = array();
    $listZone = getZoneByCode();
    if (count($listZone)){
        for ($i = 0;$i < count($listZone);$i++){
            if ($keuanganZone){
                if (in_array($listZone[$i]->zone, $getDataZone)){
                    $namaZone[$listZone[$i]->zone] = $listZone[$i]->nama;
                }
            }else{
                $namaZone[$listZone[$i]->zone] = $listZone[$i]->nama;
            }
        }
    }
    $namaNegeri = array();
    if (count($namaZone)){
        foreach ($namaZone as $idZone => $zoneName){
            $dataNegeri = get_data_negeri_by_id($idZone);
            if (count($dataNegeri)){
                for ($i = 0;$i < count($dataNegeri);$i++){
                    $namaNegeri[$dataNegeri[$i]->id] = $dataNegeri[$i]->nama;
                }
            }
        }
    }
    $dataStaff = array();
    for ($i = 0;$i < count($dataPremis);$i++){
        $ListStaff = get_data_staff_premis($dataPremis[$i]->id);
        for ($j = 0;$j < count($ListStaff);$j++){
            $dataStaff[] = $ListStaff[$j];
        }
    }
    $rows = array();
    $arrayJK = array_jenis_kelamin();
    /*count($dataStaff)*/
    for ($i = 0;$i < count($dataStaff);$i++){
        $premisData = array();
        $premisData[] = $dataStaff[$i]->nama;
        $premisData[] = $dataStaff[$i]->nama_lengkap;
        $premisData[] = $dataStaff[$i]->nama_hijrah;
        if (isset($dataStaff[$i]->jenis_kelamin) && isset($arrayJK[$dataStaff[$i]->jenis_kelamin])) {
            $premisData[] = $arrayJK[$dataStaff[$i]->jenis_kelamin];
        }else{
            $premisData[] = '-';
        }
        $premisData[] = $dataStaff[$i]->no_ktp;
        $premisData[] = $dataStaff[$i]->no_kk;
        $premisData[] = $dataStaff[$i]->telepon;
        $premisData[] = $dataStaff[$i]->alamat;
        $premisData[] = $dataStaff[$i]->tanggal_lahir;
        $premisData[] = $dataStaff[$i]->tempat_lahir;
        $umur = 0;
        $premisData[] = $umur;
        if (!empty($dataStaff[$i]->id_suami)){
            $dataSuami = get_data_staff_premis(null, $dataStaff[$i]->id_suami);
            $namaSuami = $dataSuami->nama;
        }else{
            if (!empty($dataStaff[$i]->nama_suami)){
                $namaSuami = $dataStaff[$i]->nama_suami;
            }else{
                $namaSuami = '-';
            }
        }
        $premisData[] = $namaSuami;
        if (!empty($dataStaff[$i]->id_ayah)){
            $dataAyah = get_data_staff_premis(null, $dataStaff[$i]->id_ayah);
            $namaAyah = $dataAyah->nama;
        }else{
            if (!empty($dataStaff[$i]->nama_ayah)){
                $namaAyah = $dataStaff[$i]->nama_ayah;
            }else{
                $namaAyah = '-';
            }
        }
        $premisData[] = $namaAyah;
        $premisData[] = $dataStaff[$i]->premis_nama;
        $premisData[] = $dataStaff[$i]->negri_nama;
        $premisData[] = $dataStaff[$i]->tzone_nama;
        $rows[] = $premisData;
    }
    $variables = array(
        'attributes' => $attributes,
        'header' => $header,
        'rows' => $rows,
        'footer' => null,
    );
    $listStaff = theme('datatable', $variables);
    return $listStaff;
}

function form_add_staff(){

}

function delete_staff_data(){

}

function update_staff_data(){

}

function get_data_staff_by_id(){

}