<?php

function array_jabatan_premis(){
    return array(
        'Pemantau', 'Pengurus','Wakil Pengurus','Staff','Pra-Staff','Karyawan'
    );
}
function list_premis($month = null, $year = null){
    global $user;
    if (function_exists('set_user_number_format')){
        set_user_number_format($desIn, $desOut, $separator);
    }else{
        $desIn = '.';
        $desOut = ',';
        $separator = '.';
    }
    if (!empty($month) && !empty($year)){
    }else{
        $month = date('n');
        $year = date('Y');
    }
    if (function_exists('get_total_setoran_premis')){
        $dataSetoran = get_total_setoran_premis($month, $year);
    }else{
        $dataSetoran = array();
    }
    if (function_exists('get_total_pengeluaran_oh_by_premis')){
        $dataPengeluaran = get_total_pengeluaran_oh_by_premis($month, $year);
    }else{
        $dataPengeluaran = array();
    }
    $intDate = mktime(7,0,0,$month,1,$year);
    $core_module_path = drupal_get_path('module','ikhwan_core');
    $csspath = $core_module_path.'/css/custom-style.css';
    drupal_add_css($csspath);
    $module_path  = drupal_get_path('module','data_premis');
    $jspath = $module_path.'/js/list_premis.js';
    drupal_add_js($jspath,array('scope' => 'footer', 'weight' => 5));
    // Table settings.
    $attributes['datatable_options'] = array(
        'bFilter'   => TRUE,
        'bInfo'     => TRUE,
        'bStateSave' => TRUE,
        'sPaginationType' => 'full_numbers',
        'footerCallback' => 'function(row, data, start, end, display){ alert(\'Test\'); }',
    );
    // Define table columns
    $header = array(
        array(
            'data' => t('Zone'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Nama'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        /*array(
            'data' => t('Alamat'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),*/
        array(
            'data' => t('Currency'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Setoran').' ('.date('M', $intDate).')',
            'class' => array('col-input-number align-center'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Pengeluaran').' ('.date('M', $intDate).')',
            'class' => array('col-input-number align-center'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Saldo').' ('.date('M', $intDate).')',
            'class' => array('col-input-number align-center'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Total OH'),
            'class' => array('col-input-number align-center'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Kekurangan'),
            'class' => array('col-input-number align-center'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Harian'),
            'class' => array('col-input-number-small align-center'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Edit'),
            'class' => array('col-number'),
            'datatable_options' => array(
                'bSortable' => FALSE,
                'bSearchable' => FALSE,
            ),
        ),
        array(
            'data' => t('Staff'),
            'class' => array('col-number'),
            'datatable_options' => array(
                'bSortable' => FALSE,
                'bSearchable' => FALSE,
            ),
        ),
        array(
            'data' => t('Overhead'),
            'class' => array('col-number'),
            'datatable_options' => array(
                'bSortable' => FALSE,
                'bSearchable' => FALSE,
            ),
        ),
        array(
            'data' => t('Setoran').' ('.date('M', $intDate).')',
            'class' => array('col-number'),
            'datatable_options' => array(
                'bSortable' => FALSE,
                'bSearchable' => FALSE,
            ),
        ),
        array(
            'data' => t('Hapus'),
            'class' => array('col-number'),
            'datatable_options' => array(
                'bSortable' => FALSE,
                'bSearchable' => FALSE,
            ),
        ),
    );
    // Table data.
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
    $dataOverhead = get_data_overhead();
    $arrayOH = array();
    for ($i = 0;$i < count($dataOverhead);$i++){
        $arrayOH[] = $dataOverhead[$i]->id;
    }
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
    $rows = array();
    $totalSetoran = 0;
    $totalPengeluaran = 0;
    $totalSaldo = 0;
    $gtOverhead = 0;
    $gtOverheadHarian = 0;
    $totalKekurangan = 0;
    for ($i = 0;$i < count($dataPremis);$i++){
        if ($keuanganZone){
            $detailPremis = get_data_premis_by_id(null, $dataPremis[$i]);
        }else{
            $detailPremis = $dataPremis[$i];
        }
        if (isset($detailPremis->id) && !empty($detailPremis->id)) {
            $dataStaff = get_data_staff_premis($detailPremis->id);
            $relatedStaff = array();
            $relatedStaff[] = 'UNIT';
            for ($j = 0;$j < count($dataStaff);$j++){
                $relatedStaff[] = $dataStaff[$j]->id;
            }
            $totalStaff = count($dataStaff);
            if ($totalStaff > 0) {
                $totalEntriedOH = db_query('SELECT COUNT(*) FROM {zoverhead_staff} WHERE id_premis = :id_premis', array(':id_premis' => $detailPremis->id))->fetchField();
                $totalSumOH = db_query(
                    'SELECT SUM(overhead_value) FROM {zoverhead_staff} WHERE id_premis = :id_premis 
                    AND id_staff IN (:related_staff) AND id_overhead IN (:active_overhead)',
                    array(
                        ':id_premis' => $detailPremis->id,
                        ':related_staff' => $relatedStaff,
                        ':active_overhead' => $arrayOH,
                    )
                )->fetchField();
                $totalHarianOH = round($totalSumOH / 30, 0);
            }else{
                $totalEntriedOH = 0;
                $totalSumOH = 0;
                $totalHarianOH = 0;
            }
            if ($totalEntriedOH < 2){
                $badgesOH = '<span class="badge"><i class="fa fa-close" aria-hidden="true"></i></span>';
            }else{
                $badgesOH = '<span class="badge"><i class="fa fa-check" aria-hidden="true"></i></span>';
            }
            $premisData = array();
            if (isset($namaZone[$detailPremis->zone])) {
                $premisData[] = $namaZone[$detailPremis->zone];
            } else {
                $premisData[] = '-';
            }
            $premisData[] = $detailPremis->nama;
            //$premisData[] = $detailPremis->alamat;
            $premisData[] = $detailPremis->currency;
            if (isset($dataSetoran[$detailPremis->id])){
                $setoranPremis = $dataSetoran[$detailPremis->id]->total_setoran;
            }else{
                $setoranPremis = 0;
            }
            if (isset($dataPengeluaran[$detailPremis->id])) {
                $pengeluaranPremis = $dataPengeluaran[$detailPremis->id];
            }else{
                $pengeluaranPremis = 0;
            }
            $saldo = $setoranPremis - $pengeluaranPremis;
            $totalSetoran = $totalSetoran + $setoranPremis;
            $totalPengeluaran = $totalPengeluaran + $pengeluaranPremis;
            $totalSaldo = $totalSaldo + $saldo;
            $kekurangan = $totalSumOH - $pengeluaranPremis;
            $totalKekurangan = $totalKekurangan + $kekurangan;
            $premisData[] = '<div class="align-right">'.number_format($setoranPremis,0,$desOut,$separator).'</div>';
            $premisData[] = '<div class="align-right">'.number_format($pengeluaranPremis,0,$desOut,$separator).'</div>';
            $premisData[] = '<div class="align-right">'.number_format($saldo,0,$desOut,$separator).'</div>';
            $premisData[] = '<div class="align-right">'.number_format($totalSumOH,0,$desOut,$separator).'</div>';
            $premisData[] = '<div class="align-right">'.number_format($kekurangan,0,$desOut,$separator).'</div>';
            $gtOverhead = $gtOverhead + $totalSumOH;
            $premisData[] = '<div class="align-right">'.number_format($totalHarianOH,0,$desOut,$separator).'</div>';
            $gtOverheadHarian = $gtOverheadHarian + $totalHarianOH;
            $editIcon = '<i class="fa fa-edit"></i>';
            $editButtonVar = array(
                'element' => array(
                    '#attributes' => array(
                        'class' => array('btn-info', 'btn-xs', 'no-margin', 'edit-premis'),
                        'id' => 'edit-premis-' . $detailPremis->id,
                        'name' => 'edit-premis-' . $detailPremis->id,
                        'onclick' => 'edit_premis(\'' . $detailPremis->id . '\')',
                    ),
                    '#button_type' => 'button',
                    '#value' => $editIcon . ' EDIT',
                )
            );
            $editButton = theme('button', $editButtonVar);
            $premisData[] = $editButton;
            $addIcon = '<i class="fa fa-plus"></i>';
            $staffButtonVar = array(
                'element' => array(
                    '#attributes' => array(
                        'class' => array('btn-success', 'btn-xs', 'no-margin', 'add-staff'),
                        'id' => 'add-staff-' . $detailPremis->id,
                        'name' => 'add-staff-' . $detailPremis->id,
                        'onclick' => 'manage_staff(\'' . $detailPremis->id . '\')',
                    ),
                    '#button_type' => 'button',
                    '#value' => $addIcon . ' ADD STAFF&nbsp;&nbsp;&nbsp;&nbsp;<span class="badge">' . $totalStaff . '</span>',
                )
            );
            $staffButton = theme('button', $staffButtonVar);
            $premisData[] = $staffButton;
            $ohButtonVar = array(
                'element' => array(
                    '#attributes' => array(
                        'class' => array('btn-warning', 'btn-xs', 'no-margin', 'add-oh'),
                        'id' => 'add-oh-' . $detailPremis->id,
                        'name' => 'add-oh-' . $detailPremis->id,
                        'onclick' => 'overhead_premis(\'' . $detailPremis->id . '\')',
                    ),
                    '#button_type' => 'button',
                    '#value' => $addIcon . ' OVERHEAD&nbsp;&nbsp;'.$badgesOH,
                )
            );
            $ohButton = theme('button', $ohButtonVar);
            $premisData[] = $ohButton;
            $setoranButtonVar = array(
                'element' => array(
                    '#attributes' => array(
                        'class' => array('btn-primary', 'btn-xs', 'no-margin', 'add-oh'),
                        'id' => 'add-setoran-' . $detailPremis->id,
                        'name' => 'add-setoran-' . $detailPremis->id,
                        'onclick' => 'setoran_premis(\'' . $detailPremis->id . '\','.$month.','.$year.')',
                    ),
                    '#button_type' => 'button',
                    '#value' => $addIcon . ' '.t('SETORAN').'&nbsp;&nbsp;',
                )
            );
            $setoranButton = theme('button', $setoranButtonVar);
            $premisData[] = $setoranButton;
            $delIcon = '<i class="fa fa-trash-o"></i>';
            $delButtonVar = array(
                'element' => array(
                    '#attributes' => array(
                        'class' => array('btn-danger', 'btn-xs', 'no-margin', 'del-premis'),
                        'id' => 'del-premis-' . $detailPremis->id,
                        'name' => 'del-premis-' . $detailPremis->id,
                        'onclick' => 'delete_premis(\'' . $detailPremis->id . '\')',
                    ),
                    '#button_type' => 'button',
                    '#value' => $delIcon . ' HAPUS',
                )
            );
            if ($totalStaff <= 0) {
                $delButton = theme('button', $delButtonVar);
            } else {
                $delButton = '';
            }

            $premisData[] = $delButton;
            $rows[] = $premisData;
        }
    }
    $footer[] = array(
        array(
            'data' => '<strong>'.t('TOTAL').'</strong>',
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
            'colspan' => 3,
            'class' => array('align-right'),
        ),
        array(
            'data' => number_format($totalSetoran,0,$desOut,$separator),
            'class' => array('col-input-number align-right'),
        ),
        array(
            'data' => number_format($totalPengeluaran,0,$desOut,$separator),
            'class' => array('col-input-number align-right'),
        ),
        array(
            'data' => number_format($totalSaldo,0,$desOut,$separator),
            'class' => array('col-input-number align-right'),
        ),
        array(
            'data' => number_format($gtOverhead,0,$desOut,$separator),
            'class' => array('col-input-number align-right'),
        ),
        array(
            'data' => number_format($totalKekurangan,0,$desOut,$separator),
            'class' => array('col-input-number align-right'),
        ),
        array(
            'data' => number_format($gtOverheadHarian,0,$desOut,$separator),
            'class' => array('col-input-number-small align-right'),
        ),
        array(
            'data' => '&nbsp;',
            'colspan' => 5,
        ),
    );
    // Or, render using a theme function.
    $variables = array(
        'attributes' => $attributes,
        'header' => $header,
        'rows' => $rows,
        'footer' => $footer,
    );
    $newPremisButtonVar = array(
        'element' => array(
            '#attributes' => array(
                'class' => array('btn-info btn-sm'),
                'id' => 'new-premis',
                'name' => 'new-premis',
            ),
            '#button_type' => 'button',
            '#value' => t('Tambah Data Premis'),
        )
    );
    $newPremisButton = theme('button', $newPremisButtonVar);
    $variables['content'] = t('Pilih Bulan dan Tahun Setoran & Pengeluaran, kemudian klik tombol show untuk melihat Setoran dan Pengeluaran');
    $variables['month'] = $month;
    $variables['year'] = $year;
    $variables['alamat'] = 'masterdata/premis';
    $form = drupal_get_form('filter_periode_form',$variables);
    $formSetoran = drupal_render($form);
    $listPremis = $newPremisButton.$formSetoran.theme('datatable', $variables);
    return $listPremis;
}

function form_add_premis($idPremis = null){
    $module_path = drupal_get_path('module','ikhwan_core');
    $csspath = $module_path.'/css/custom-style.css';
    drupal_add_css($csspath);
    $getDataPremis = null;
    if (!empty($idPremis)){
        $getDataPremis = get_data_premis_by_id(null, $idPremis);
    }
    if (empty($getDataPremis)){
        $getDataPremis = new stdClass();
        $getDataPremis->zone = '';
        $getDataPremis->nama = '';
        $getDataPremis->currency = variable_get('google_currency_converter_to', '');
        $getDataPremis->alamat = '';
        $getDataPremis->telepon = '';
        $getDataPremis->whatsapp = '';
        $getDataPremis->bbm = '';
        $getDataPremis->telegram = '';
        $getDataPremis->email = '';
    }
    $variables = $getDataPremis;
    $form = drupal_get_form('form_data_premis',$variables);
    $formPremis = drupal_render($form);
    return $formPremis;
}

function get_data_premis_by_id($zone = null, $idpremis = null){
    $dataReturn = null;
    if (!empty($idpremis)){
        $fieldArray = array(
            'id', 'zone', 'nama', 'alamat', 'telepon', 'whatsapp', 'bbm',
            'telegram', 'email', 'created', 'changed', 'uid', 'uid_changed', 'currency'
        );
        $dataPremis = getTableDataByField('zdatapremis','access data premis',$fieldArray,null,'id',$idpremis);
        if (count($dataPremis)){
            $dataReturn = $dataPremis[0];
        }
    }else{
        $fieldArray = array(
            'id', 'zone', 'nama', 'alamat', 'telepon', 'whatsapp', 'bbm',
            'telegram', 'email', 'created', 'changed', 'uid', 'uid_changed', 'currency'
        );
        if (empty($zone)){
            $dataPremis = getTableDataByField('zdatapremis','access data premis',$fieldArray,'nama');
        }else{
            $consData = new stdClass();
            $consData->fieldName = 'ta.zone';
            $consData->value = $zone;
            if (is_array($zone)){
                $consData->operator = 'IN';
            }
            $conditionArray[] = $consData;
            $dataPremis = getTableDataByField(
                'zdatapremis','access data premis',$fieldArray,
                'nama',null,null,null,$conditionArray
            );
        }
        if (count($dataPremis)){
            $dataReturn = $dataPremis;
        }
    }
    return $dataReturn;
}
function form_data_premis($form, &$form_state, $variables = null){
    global $user;
    $form['#attributes'] = array('class' => array('col-sm-12'));
    $form['basic'] = array(
        '#type' => 'fieldset',
        '#title' => t('Premis Info'),
        '#collapsible' => TRUE, // Added
        '#collapsed' => FALSE,  // Added
        '#prefix' => '<div class="col-md-6">',
    );
    $getDataZone = getZoneByCode();
    $keuanganZone = cek_keuangan_zone();
    $userZone = get_user_zone();
    $arrayZone = array();
    for ($i = 0;$i < count($getDataZone);$i++){
        if ($userZone[0]){
            if (in_array($getDataZone[$i]->zone, $userZone[1]['zone'])){
                $arrayZone[$getDataZone[$i]->zone] = $getDataZone[$i]->nama;
            }
        }else{
            $arrayZone[$getDataZone[$i]->zone] = $getDataZone[$i]->nama;
        }
    }
    if (!empty($variables->id)){
        $form['basic']['id'] = array(
            '#type' => 'hidden',
            '#default_value' => $variables->id,
        );
    }
    $form['basic']['zone'] = array(
        '#title' => t('Zone'),
        '#type' => 'select',
        '#required' => TRUE,
        '#options' => $arrayZone,
        '#default_value' => $variables->zone,
        '#select2' => array(
            'placeholder' => 'Select Zone',
        ),
    );
    $form['basic']['nama'] = array(
        '#type' => 'textfield',
        '#title' => t('Nama Premis'),
        '#required' => TRUE,
        '#default_value' => $variables->nama,
    );
    $form['basic']['currency'] = array(
        '#type' => 'select',
        '#title' => t('Currency'),
        '#options' => google_currency_converter_country_list(),
        '#default_value' => $variables->currency,
        '#attributes' => array('class' => array('google-currency-converter-element')),
        '#required' => TRUE,
    );
    $form['basic']['alamat'] = array(
        '#title' => t('Alamat'),
        '#type' => 'textarea',
        '#default_value' => $variables->alamat,
    );
    $form['contact'] = array(
        '#type' => 'fieldset',
        '#title' => t('Premis Contact'),
        '#collapsible' => TRUE, // Added
        '#collapsed' => FALSE,  // Added
        '#prefix' => '</div><div class="col-md-6">',
    );
    $form['contact']['telepon'] = array(
        '#type' => 'textfield',
        '#title' => t('Telephone'),
        '#default_value' => $variables->telepon,
    );
    $form['contact']['whatsapp'] = array(
        '#type' => 'textfield',
        '#title' => t('Whatsapp'),
        '#default_value' => $variables->whatsapp,
    );
    $form['contact']['bbm'] = array(
        '#type' => 'textfield',
        '#title' => t('BBM'),
        '#default_value' => $variables->bbm,
    );
    $form['contact']['telegram'] = array(
        '#type' => 'textfield',
        '#title' => t('Telegram'),
        '#default_value' => $variables->telegram,
    );
    $form['contact']['email'] = array(
        '#type' => 'textfield',
        '#title' => t('Email'),
        '#default_value' => $variables->email,
        '#suffix' => '</div>',
    );
    $form['saveButton'] = array(
        '#value' => t('Save Data Premis'),
        '#type' => 'submit',
        '#attributes' => array('class' => array('btn btn-primary')),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>',
    );
    return $form;
}

function form_data_premis_submit($form, &$form_state) {
    global $user;
    $values = $form_state['values'];
    $savedData = array();
    $savedData['zone'] = $values['zone'];
    $savedData['nama'] = $values['nama'];
    $savedData['alamat'] = $values['alamat'];
    $savedData['telepon'] = $values['telepon'];
    $savedData['whatsapp'] = $values['whatsapp'];
    $savedData['bbm'] = $values['bbm'];
    $savedData['telegram'] = $values['telegram'];
    $savedData['email'] = $values['email'];
    $savedData['currency'] = $values['currency'];
    if (isset($values['id']) && !empty($values['id'])){
        if (!empty($savedData['nama'])){
            $updatedData = saveTableData('zdatapremis','admin data premis',$savedData,'id',$values['id']);
            $returnData = $updatedData;
            drupal_set_message("Data premis berhasil di update");
        }else{
            $dataStaff = get_data_staff_premis($values['id']);
            if (!count($dataStaff)){
                $conditionData = array();
                $consObj = new stdClass();
                $consObj->fieldName = 'id';
                $consObj->fieldValue = $values['id'];
                $consObj->status = 'and';
                $conditionData[] = $consObj;
                deleteTableData('zdatapremis', 'admin data premis',$conditionData);
            }
            drupal_set_message("Data premis berhasil di hapus",'error');
        }
        drupal_goto('masterdata/premis');
    }else{
        $insertedData = saveTableData('zdatapremis','admin data premis',$savedData,'id');
        $returnData = $insertedData;
        drupal_set_message("Data premis berhasil di simpan");
    }
    $fieldArray = array(
        'id', 'nama', 'related_uid', 'zone',
        'id_premis', 'uid', 'uid_changed', 'created', 'changed'
    );
    $userZonePremis = getTableDataByField(
        'zuser_zone', 'admin data premis',
        $fieldArray, 'nama', 'related_uid', $user->uid
    );
    if (count($userZonePremis)){
        $premisData = explode(',',$userZonePremis[0]->id_premis);
        $premisData[] = $returnData['id'];
        $userPremisUpdate['id'] = $userZonePremis[0]->id;
        $userPremisUpdate['nama'] = $userZonePremis[0]->nama;
        $userPremisUpdate['related_uid'] = $userZonePremis[0]->related_uid;
        $userPremisUpdate['zone'] = $userZonePremis[0]->zone;
        $userPremisUpdate['id_premis'] = implode(',',$premisData);
        $userPremisUpdate['uid_changed'] = $user->uid;
        $userPremisUpdate['changed'] = time();
        saveTableData('zuser_zone','admin data premis',$userPremisUpdate,'id',$userPremisUpdate['id']);
    }
    return $returnData;
}
function get_data_staff_premis($idPremis = null, $idStaff = null){
    $dataStaff = null;
    if (!empty($idPremis) && empty($idStaff)){
        $fieldArray = array(
            'id', 'id_premis', 'id_jawatan', 'nama', 'alamat', 'telepon', 'whatsapp', 'bbm',
            'telegram', 'email', 'created', 'changed', 'uid', 'uid_changed'
        );
        $dataStaff = getTableDataByField('zdatastaff','access data premis',$fieldArray,'id_jawatan','id_premis',$idPremis);
    }else if (!empty($idStaff)){
        $fieldArray = array(
            'id', 'id_premis', 'id_jawatan', 'nama', 'alamat', 'telepon', 'whatsapp', 'bbm',
            'telegram', 'email', 'created', 'changed', 'uid', 'uid_changed'
        );
        $dataStaff = getTableDataByField('zdatastaff','access data premis',$fieldArray,'id_jawatan','id',$idStaff);
    }else if (empty($idPremis) && empty($idStaff)){
        $fieldArray = array(
            'id', 'id_premis', 'id_jawatan', 'nama', 'alamat', 'telepon', 'whatsapp', 'bbm',
            'telegram', 'email', 'created', 'changed', 'uid', 'uid_changed'
        );
        $dataStaff = getTableDataByField('zdatastaff','access data premis',$fieldArray,'id_jawatan');
    }
    return $dataStaff;
}
function create_staff_premis_header(){
    $tableHeader = array();
    $tableHeader[] = array(
        'data' => t('No.'),
        'class' => array(
            'col-number'
        ),
    );
    $tableHeader[] = t('NAMA');
    $tableHeader[] = t('JAWATAN');
    $tableHeader[] = t('ALAMAT');
    $tableHeader[] = t('TELEPON');
    $tableHeader[] = t('EMAIL');
    return $tableHeader;
}
function create_staff_premis_row($idpremis = null){
    $tableRow = array();
    $tableRow['#tree'] = TRUE;
    $dataStaff = null;
    if (!empty($idpremis)){
        $dataStaff = get_data_staff_premis($idpremis);
    }
    if (!is_null($dataStaff) && count($dataStaff)){
        for ($i = 0;$i < count($dataStaff);$i++){
            $colData = array();
            $colData['bil-'.$dataStaff[$i]->id.'-'.$idpremis.'-edit'] = array(
                '#type' => 'item', '#title' => ($i + 1),
                '#attributes' => array('class' => array('align-right')),
            );
            $colData['nama-'.$dataStaff[$i]->id.'-'.$idpremis.'-edit'] = array(
                '#type' => 'textfield',
                '#default_value' => $dataStaff[$i]->nama,
            );
            $colData['id_jawatan-'.$dataStaff[$i]->id.'-'.$idpremis.'-edit'] = array(
                '#type' => 'select',
                '#default_value' => $dataStaff[$i]->id_jawatan,
                '#options' => array_jabatan_premis(),
                '#select2' => array(
                    'placeholder' => 'Pilih Jawatan',
                ),
            );
            $colData['alamat-'.$dataStaff[$i]->id.'-'.$idpremis.'-edit'] = array(
                '#type' => 'textfield',
                '#default_value' => $dataStaff[$i]->alamat,
            );
            $colData['telepon-'.$dataStaff[$i]->id.'-'.$idpremis.'-edit'] = array(
                '#type' => 'textfield',
                '#default_value' => $dataStaff[$i]->telepon,
            );
            $colData['email-'.$dataStaff[$i]->id.'-'.$idpremis.'-edit'] = array(
                '#type' => 'textfield',
                '#default_value' => $dataStaff[$i]->email,
            );
            $tableRow['r'.($i + 1)] = $colData;
        }
        $nextCounter = $i + 1;
        for ($i = 0;$i < 3;$i++){
            $colData = array();
            $colData['bil-new-'.$nextCounter.'-'.$idpremis] = array(
                '#type' => 'item', '#title' => ($nextCounter),
                '#attributes' => array('outerclass' => array('align-right')),
            );
            $colData['nama-new-'.$nextCounter.'-'.$idpremis] = array(
                '#type' => 'textfield',
                '#default_value' => '',
            );
            $colData['id_jawatan-new-'.$nextCounter.'-'.$idpremis] = array(
                '#type' => 'select',
                '#default_value' => 3,
                '#options' => array_jabatan_premis(),
                '#select2' => array(
                    'placeholder' => 'Pilih Jawatan',
                ),
            );
            $colData['alamat-new-'.$nextCounter.'-'.$idpremis] = array(
                '#type' => 'textfield',
                '#default_value' => '',
            );
            $colData['telepon-new-'.$nextCounter.'-'.$idpremis] = array(
                '#type' => 'textfield',
                '#default_value' => '',
            );
            $colData['email-new-'.$nextCounter.'-'.$idpremis] = array(
                '#type' => 'textfield',
                '#default_value' => '',
            );
            $tableRow['r'.$nextCounter] = $colData;
            $nextCounter++;
        }
    }else{
        $nextCounter = 1;
        for ($i = 0;$i < 5;$i++){
            $colData = array();
            $colData['bil-new-'.$nextCounter.'-'.$idpremis] = array(
                '#type' => 'item', '#title' => ($nextCounter),
                '#attributes' => array('outerclass' => array('align-right')),
            );
            $colData['nama-new-'.$nextCounter.'-'.$idpremis] = array(
                '#type' => 'textfield',
                '#default_value' => '',
            );
            $colData['id_jawatan-new-'.$nextCounter.'-'.$idpremis] = array(
                '#type' => 'select',
                '#default_value' => 3,
                '#options' => array_jabatan_premis(),
                '#select2' => array(
                    'placeholder' => 'Pilih Jawatan',
                ),
            );
            $colData['alamat-new-'.$nextCounter.'-'.$idpremis] = array(
                '#type' => 'textfield',
                '#default_value' => '',
            );
            $colData['telepon-new-'.$nextCounter.'-'.$idpremis] = array(
                '#type' => 'textfield',
                '#default_value' => '',
            );
            $colData['email-new-'.$nextCounter.'-'.$idpremis] = array(
                '#type' => 'textfield',
                '#default_value' => '',
            );
            $tableRow['r'.$nextCounter] = $colData;
            $nextCounter++;
        }
    }
    return $tableRow;
}
function premis_staff_form($form, &$form_state, $variables = null){
    set_default_time_zone();
    if (isset($variables['idpremis']) && !empty($variables['idpremis'])){
        $tableHeader = create_staff_premis_header();
        $dataPremis = get_data_premis_by_id(null,$variables['idpremis']);
        $tableRow = create_staff_premis_row($variables['idpremis']);
        $tableFooter = array(create_staff_premis_header());
        $form['basic'] = array(
            '#type' => 'fieldset',
            '#title' => 'DATA STAFF PREMIS '.$dataPremis->nama,
            '#collapsible' => FALSE, // Added
            '#collapsed' => FALSE,  // Added
        );
        $form['basic']['table'] = array(
            '#theme' => 'formtable_form_table',
            '#header' => $tableHeader,
            'rows' => $tableRow,
            '#footer' => $tableFooter,
        );
        $form['basic']['submit'] = array(
            '#type' => 'submit',
            '#value' => t('SAVE DATA STAFF'),
        );
    }
    return $form;
}
function add_premis_staff($idPremis = null){
    if (!empty($idPremis)){
        $variables['idpremis'] = $idPremis;
        $form = drupal_get_form('premis_staff_form',$variables);
        $formStaff = drupal_render($form);
        return $formStaff;
    }else{
        drupal_set_message(t('Mohon Tambah Staff Dari Data Premis...!!!'));
        drupal_goto('masterdata/premis');
    }
}
function premis_staff_form_submit($form, &$form_state){
    global $user;
    if (isset($form_state['values']['rows'])) {
        $values = $form_state['values']['rows'];
    } else {
        $values = $form_state['values'];
    }
    $returnData = array();
    if (count($values)) {
        foreach ($values as $idx => $rowData) {
            if (count($rowData)){
                $savedData = array();
                foreach ($rowData as $idxData => $colValues){
                    $splitData = explode('-',$idxData);
                    if ($splitData[1] == 'new'){
                        $savedData[$splitData[0]] =  $colValues;
                        $savedData['id_premis'] =  $splitData[3];
                    }else{
                        $savedData['id'] =  $splitData[1];
                        $savedData[$splitData[0]] =  $colValues;
                        $savedData['id_premis'] =  $splitData[2];
                    }
                }
                if (!isset($savedData['id'])){
                    if (isset($savedData['nama']) && !empty($savedData['nama'])) {
                        $returnData[] = saveTableData(
                            'zdatastaff', 'admin data premis',
                            $savedData, 'id'
                        );
                    }
                }else{
                    if (isset($savedData['nama']) && !empty($savedData['nama'])) {
                        $returnData[] = saveTableData(
                            'zdatastaff', 'admin data premis',
                            $savedData, 'id', $savedData['id']
                        );
                    }else{
                        $conditionData = array();
                        $consObj = new stdClass();
                        $consObj->fieldName = 'id';
                        $consObj->fieldValue = $savedData['id'];
                        $consObj->status = 'and';
                        $conditionData[] = $consObj;
                        deleteTableData('zdatastaff', 'admin data premis',$conditionData);
                    }
                }
            }
        }
    }
    if (count($returnData)){
        drupal_set_message('Data Staff Berhasil Disimpan..!!');
    }
    return $returnData;
}
function delete_premis_data($idPremis = null){
    $deletedData = null;
    if (!empty($idPremis)){
        $conditionData = array();
        $consObj = new stdClass();
        $consObj->fieldName = 'id';
        $consObj->fieldValue = $idPremis;
        $consObj->status = 'and';
        $conditionData[] = $consObj;
        $deletedData = deleteTableData('zdatapremis', 'admin data premis',$conditionData);
        drupal_set_message('Premis berhasil dihapus...!!','error');
        drupal_goto('masterdata/premis');
    }
    return $deletedData;
}
function get_data_premis_by_zone_ajax(){
    $zone = $_REQUEST['zone'];
    $dataPremis = get_data_premis_by_id($zone);
    $arrPremis = array();
    for ($i = 0;$i < count($dataPremis);$i++){
        $arrPremis[$dataPremis[$i]->id] = $dataPremis[$i]->nama;
    }
    print json_encode($arrPremis);
    exit();
}