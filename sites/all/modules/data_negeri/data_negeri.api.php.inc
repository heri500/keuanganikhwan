<?php

function list_negeri($month = null, $year = null)
{
    global $user;
    $core_module_path = drupal_get_path('module', 'ikhwan_core');
    $csspath = $core_module_path . '/css/custom-style.css';
    drupal_add_css($csspath);
    $module_path = drupal_get_path('module', 'data_negeri');
    $jspath = $module_path . '/js/list_negeri.js';
    drupal_add_js($jspath, array('scope' => 'footer', 'weight' => 5));
    // Table settings.
    $attributes['datatable_options'] = array(
        'bFilter' => TRUE,
        'bInfo' => TRUE,
        'bStateSave' => TRUE,
        'sPaginationType' => 'full_numbers',
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
        array(
            'data' => t('Premis'),
            'datatable_options' => array(
                'bSortable' => FALSE,
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
            'data' => t('Hapus'),
            'class' => array('col-number'),
            'datatable_options' => array(
                'bSortable' => FALSE,
                'bSearchable' => FALSE,
            ),
        ),
    );
    // Table data.
    $rows = array();
    $dataNegeri = get_data_negeri_by_id();
    $listZone = getZoneByCode();
    if (count($listZone)) {
        for ($i = 0; $i < count($listZone); $i++) {
            $namaZone[$listZone[$i]->zone] = $listZone[$i]->nama;
        }
    }
    for ($i = 0; $i < count($dataNegeri); $i++) {
        $negeriData = array();
        if (isset($namaZone[$dataNegeri[$i]->zone])) {
            $negeriData[] = $namaZone[$dataNegeri[$i]->zone];
        } else {
            $negeriData[] = '-';
        }
        $negeriData[] = $dataNegeri[$i]->nama;
        $dataPremis = get_data_premis_negeri($dataNegeri[$i]->id);
        $premisList = '';
        for ($j = 0;$j < count($dataPremis);$j++){
            if ($premisList == ''){
                $premisList .= $dataPremis[$j]->nama;
            }else {
                $premisList .= ', ' . $dataPremis[$j]->nama;
            }
        }
        $negeriData[] = $premisList;
        $editIcon = '<i class="fa fa-edit"></i>';
        $editButtonVar = array(
            'element' => array(
                '#attributes' => array(
                    'class' => array('btn-info', 'btn-xs', 'no-margin', 'edit-negeri'),
                    'id' => 'edit-negeri-' . $dataNegeri[$i]->id,
                    'name' => 'edit-negeri-' . $dataNegeri[$i]->id,
                    'onclick' => 'edit_negeri(\'' . $dataNegeri[$i]->id . '\')',
                ),
                '#button_type' => 'button',
                '#value' => $editIcon . ' EDIT',
            )
        );
        $editButton = theme('button', $editButtonVar);
        $negeriData[] = $editButton;
        $delIcon = '<i class="fa fa-trash-o"></i>';
        $delButtonVar = array(
            'element' => array(
                '#attributes' => array(
                    'class' => array('btn-danger', 'btn-xs', 'no-margin', 'del-negeri'),
                    'id' => 'del-negeri-' . $dataNegeri[$i]->id,
                    'name' => 'del-negeri-' . $dataNegeri[$i]->id,
                    'onclick' => 'delete_negeri(\'' . $dataNegeri[$i]->id . '\')',
                ),
                '#button_type' => 'button',
                '#value' => $delIcon . ' HAPUS',
            )
        );
        if (count($dataPremis) <= 0) {
            $delButton = theme('button', $delButtonVar);
        } else {
            $delButton = '';
        }

        $negeriData[] = $delButton;
        $rows[] = $negeriData;
    }
    // Or, render using a theme function.
    $variables = array(
        'attributes' => $attributes,
        'header' => $header,
        'rows' => $rows,
    );
    $newNegeriButtonVar = array(
        'element' => array(
            '#attributes' => array(
                'class' => array('btn-info btn-sm'),
                'id' => 'new-negeri',
                'name' => 'new-negeri',
            ),
            '#button_type' => 'button',
            '#value' => t('Tambah Data Negeri'),
        )
    );
    $newNegeriButton = theme('button', $newNegeriButtonVar);
    $listNegeri = $newNegeriButton.theme('datatable', $variables);
    return $listNegeri;
}

function form_add_negeri($idNegeri = null){
    $module_path = drupal_get_path('module','ikhwan_core');
    $csspath = $module_path.'/css/custom-style.css';
    drupal_add_css($csspath);
    $getDataNegeri = null;
    if (!empty($idNegeri)){
        $getDataNegeri = get_data_negeri_by_id(null, $idNegeri);
    }
    if (empty($getDataNegeri)){
        $getDataNegeri = new stdClass();
        $getDataNegeri->zone = '';
        $getDataNegeri->nama = '';
        $getDataNegeri->alamat = '';
        $getDataNegeri->telepon = '';
        $getDataNegeri->email = '';
    }
    $variables = $getDataNegeri;
    $form = drupal_get_form('form_data_negeri',$variables);
    $formNegeri = drupal_render($form);
    return $formNegeri;
}

function get_data_negeri_by_id($zone = null, $idnegeri = null){
    $dataReturn = null;
    if (!empty($idnegeri)){
        $fieldArray = array(
            'id', 'zone', 'nama', 'alamat', 'telepon',
            'email', 'created', 'changed', 'uid', 'uid_changed'
        );
        $dataNegeri = getTableDataByField('zdatanegeri','access data negeri',$fieldArray,null,'id',$idnegeri);
        if (count($dataNegeri)){
            $dataReturn = $dataNegeri[0];
        }
    }else{
        $fieldArray = array(
            'id', 'zone', 'nama', 'alamat', 'telepon',
            'email', 'created', 'changed', 'uid', 'uid_changed'
        );
        if (empty($zone)){
            $dataNegeri = getTableDataByField('zdatanegeri','access data negeri',$fieldArray,'nama');
        }else{
            $consData = new stdClass();
            $consData->fieldName = 'ta.zone';
            $consData->value = $zone;
            if (is_array($zone)){
                $consData->operator = 'IN';
            }
            $conditionArray[] = $consData;
            $dataNegeri = getTableDataByField(
                'zdatanegeri','access data negeri',$fieldArray,
                'nama',null,null,null,$conditionArray
            );
        }
        if (count($dataNegeri)){
            $dataReturn = $dataNegeri;
        }
    }
    return $dataReturn;
}
function form_data_negeri($form, &$form_state, $variables = null){
    global $user;
    $form['#attributes'] = array('class' => array('col-sm-12'));
    $form['basic'] = array(
        '#type' => 'fieldset',
        '#title' => t('Negeri Info'),
        '#collapsible' => TRUE, // Added
        '#collapsed' => FALSE,  // Added
        '#prefix' => '<div class="col-md-6">',
    );
    $getDataZone = getZoneByCode();
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
        '#title' => t('Nama Negeri'),
        '#required' => TRUE,
        '#default_value' => $variables->nama,
    );
    $form['basic']['alamat'] = array(
        '#title' => t('Alamat'),
        '#type' => 'textarea',
        '#default_value' => $variables->alamat,
    );
    $form['contact'] = array(
        '#type' => 'fieldset',
        '#title' => t('Negeri Contact'),
        '#collapsible' => TRUE, // Added
        '#collapsed' => FALSE,  // Added
        '#prefix' => '</div><div class="col-md-6">',
    );
    $form['contact']['telepon'] = array(
        '#type' => 'textfield',
        '#title' => t('Telephone'),
        '#default_value' => $variables->telepon,
    );
    $form['contact']['email'] = array(
        '#type' => 'textfield',
        '#title' => t('Email'),
        '#default_value' => $variables->email,
        '#suffix' => '</div>',
    );
    $form['saveButton'] = array(
        '#value' => t('Save Data Negeri'),
        '#type' => 'submit',
        '#attributes' => array('class' => array('btn btn-primary')),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>',
    );
    return $form;
}

function form_data_negeri_submit($form, &$form_state) {
    global $user;
    $values = $form_state['values'];
    $savedData = array();
    $savedData['zone'] = $values['zone'];
    $savedData['nama'] = $values['nama'];
    $savedData['alamat'] = $values['alamat'];
    $savedData['telepon'] = $values['telepon'];
    $savedData['email'] = $values['email'];
    if (isset($values['id']) && !empty($values['id'])){
        if (!empty($savedData['nama'])){
            $updatedData = saveTableData('zdatanegeri','admin data negeri',$savedData,'id',$values['id']);
            $returnData = $updatedData;
            drupal_set_message("Data negeri berhasil di update");
        }else{
            $dataPremis = get_data_premis_negeri($values['id']);
            if (!count($dataPremis)){
                $conditionData = array();
                $consObj = new stdClass();
                $consObj->fieldName = 'id';
                $consObj->fieldValue = $values['id'];
                $consObj->status = 'and';
                $conditionData[] = $consObj;
                deleteTableData('zdatanegeri', 'admin data negeri',$conditionData);
            }
            drupal_set_message("Data negeri berhasil di hapus",'error');
        }
        drupal_goto('masterdata/negeri');
    }else{
        $insertedData = saveTableData('zdatanegeri','admin data negeri',$savedData,'id');
        $returnData = $insertedData;
        drupal_set_message("Data negeri berhasil di simpan");
    }
    return $returnData;
}
function get_data_premis_negeri($idNegeri = null, $idPremis = null){
    $dataPremis = null;
    if (!empty($idNegeri) && empty($idPremis)){
        $fieldArray = array(
            'id', 'idnegeri', 'nama', 'alamat', 'telepon', 'whatsapp', 'bbm',
            'telegram', 'email', 'created', 'changed', 'uid', 'uid_changed'
        );
        $dataPremis = getTableDataByField('zdatapremis','access data negeri',$fieldArray,null,'idnegeri',$idNegeri);
    }else if (!empty($idPremis)){
        $fieldArray = array(
            'id', 'idnegeri', 'nama', 'alamat', 'telepon', 'whatsapp', 'bbm',
            'telegram', 'email', 'created', 'changed', 'uid', 'uid_changed'
        );
        $dataPremis = getTableDataByField('zdatapremis','access data negeri',$fieldArray,null,'id',$idPremis);
    }else if (empty($idNegeri) && empty($idStaff)){
        $fieldArray = array(
            'id', 'idnegeri', 'nama', 'alamat', 'telepon', 'whatsapp', 'bbm',
            'telegram', 'email', 'created', 'changed', 'uid', 'uid_changed'
        );
        $dataPremis = getTableDataByField('zdatapremis','access data negeri',$fieldArray);
    }
    return $dataPremis;
}

function delete_negeri_data($idNegeri = null){
    $deletedData = null;
    if (!empty($idNegeri)){
        $conditionData = array();
        $consObj = new stdClass();
        $consObj->fieldName = 'id';
        $consObj->fieldValue = $idNegeri;
        $consObj->status = 'and';
        $conditionData[] = $consObj;
        $deletedData = deleteTableData('zdatanegeri', 'admin data negeri',$conditionData);
        drupal_set_message('Negeri berhasil dihapus...!!','error');
        drupal_goto('masterdata/negeri');
    }
    return $deletedData;
}
function get_data_negeri_by_zone_ajax(){
    $zone = $_REQUEST['zone'];
    $dataNegeri = get_data_negeri_by_id($zone);
    $arrNegeri = array();
    for ($i = 0;$i < count($dataNegeri);$i++){
        $arrNegeri[$dataNegeri[$i]->id] = $dataNegeri[$i]->nama;
    }
    print json_encode($arrNegeri);
    exit();
}