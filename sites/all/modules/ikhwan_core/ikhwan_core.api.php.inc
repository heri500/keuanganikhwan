<?php
/**
 * @return array of zone
 */
function array_zone_gisb(){
    return array(
        '001' => 'Utara',
        '002' => 'Kedah',
        '003' => 'PKBM',
        '004' => 'Penang',
        '005' => 'Perak',
        '006' => 'Barat',
        '007' => 'Pusat',
        '008' => 'Tengah',
        '009' => 'Utama',
        '010' => 'Eksekutif',
        '011' => 'Yayasan',
        '012' => 'Negeri Sembilan',
        '013' => 'Melaka',
        '014' => 'Johor 1',
        '015' => 'Johor 2',
        '016' => 'Pahang',
        '017' => 'Trengganu',
        '018' => 'Kelantan',
        '019' => 'Sabah 1/3',
        '020' => 'Sabah 2/4',
        '021' => 'Sarawak 1',
        '022' => 'Sarawak 2',
        '023' => 'Labuan',
        '024' => 'Thailand 1',
        '025' => 'Thailand 2',
        '026' => 'Thailand 3',
        '027' => 'Pendidikan Insaniah',
        '028' => 'Pendidikan Vokasional',
        '029' => 'Australia T',
        '030' => 'Australia B',
        '031' => 'Sumatera 1',
        '032' => 'Sumatera 2',
        '033' => 'Sumatera 3',
        '034' => 'Jawa  1/2',
        '035' => 'Jawa  3/4',
        '036' => 'Makassar',
        '037' => 'Papua',
        '038' => 'Syam',
        '039' => 'Afrika Utara',
        '040' => 'Haramain',
        '041' => 'Mesir',
    );
}
function set_default_time_zone(){
    $defaultTimeZone = date_default_timezone_get();
    if ($defaultTimeZone != 'Asia/Jakarta'){
        date_default_timezone_set('Asia/Jakarta');
    }
}
/**
 * @param null $tableName
 * @param null $userAccess
 * @param null $fieldArray
 * @param null $fieldOrder
 * @param null $fieldCondition
 * @param null $fieldConditionValue
 * @param null $leftJoinArray
 * @param null $conditionArray
 * @param null $range
 *
 * @return $this|null|\SelectQuery|\SelectQueryInterface
 */
function getTableDataByField($tableName = null, $userAccess = null, $fieldArray = null, $fieldOrder = null, $fieldCondition = null, $fieldConditionValue = null, $leftJoinArray = null, $conditionArray = null, $range = null){
    $query = null;
    if (!is_null($tableName) && !is_null($fieldArray) && !is_null($userAccess)){
        if (user_access($userAccess)) {
            $strField = implode(',', $fieldArray);
            $query = db_select($tableName,'ta');
            if ($leftJoinArray !== null && count($leftJoinArray)){
                for ($i = 0;$i < count($leftJoinArray);$i++){
                    $leftJoinData = $leftJoinArray[$i];
                    if (isset($leftJoinData['sourcealias'])){
                        $query->leftJoin($leftJoinData['tablename'], $leftJoinData['tablealias'], $leftJoinData['sourcealias'].'.'.$leftJoinData['field_join'].' = '.$leftJoinData['tablealias'].'.'.$leftJoinData['field_source']);
                    }else{
                        $query->leftJoin($leftJoinData['tablename'], $leftJoinData['tablealias'], 'ta.'.$leftJoinData['field_join'].' = '.$leftJoinData['tablealias'].'.'.$leftJoinData['field_source']);
                    }
                }
            }
            $query = $query->fields('ta', $fieldArray);
            if (!is_null($leftJoinArray) && count($leftJoinArray)){
                for ($i = 0;$i < count($leftJoinArray);$i++){
                    $leftJoinData = $leftJoinArray[$i];
                    $query = $query->fields($leftJoinData['tablealias'], $leftJoinData['fieldArray']);
                }
            }
            if (!is_null($fieldOrder)){
                if (!is_array($fieldOrder)){
                    $explodeFieldOrder = explode('.', $fieldOrder);
                    if (count($explodeFieldOrder)){
                        $query = $query->orderBy($fieldOrder);
                    }else{
                        $query = $query->orderBy('ta.'.$fieldOrder);
                    }
                }else{
                    for ($i = 0;$i < count($fieldOrder);$i++){
                        if (isset($fieldOrder[$i]['operator'])){
                            $explodeFieldOrder = explode('.', $fieldOrder[$i]['fieldname']);
                            if (count($explodeFieldOrder)){
                                $query = $query->orderBy($fieldOrder[$i]['fieldname'], $fieldOrder[$i]['operator']);
                            }else{
                                $query = $query->orderBy('ta.'.$fieldOrder[$i]['fieldname'], $fieldOrder[$i]['operator']);
                            }
                        }else{
                            $explodeFieldOrder = explode('.', $fieldOrder[$i]['fieldname']);
                            if (count($explodeFieldOrder)){
                                $query = $query->orderBy($fieldOrder[$i]['fieldname']);
                            }else{
                                $query = $query->orderBy('ta.'.$fieldOrder[$i]['fieldname']);
                            }
                        }
                    }
                }
            }
            if (!is_null($fieldCondition) && !is_null($fieldConditionValue)){
                $query = $query->condition('ta.'.$fieldCondition, $fieldConditionValue);
            }
            if (!is_null($conditionArray) && count($conditionArray)){
                $db_or = db_or();
                $db_and = db_and();
                for ($i = 0;$i < count($conditionArray);$i++){
                    if (isset($conditionArray[$i]->connector)){
                        if ($conditionArray[$i]->connector == 'OR'){
                            $conditionField = $conditionArray[$i]->fieldName;
                            $conditionValue = $conditionArray[$i]->value;
                            if (isset($conditionArray[$i]->operator) && !empty($conditionArray[$i]->operator)){
                                $conditionOperator = $conditionArray[$i]->operator;
                            }else{
                                $conditionOperator = '=';
                            }
                            $db_or->condition($conditionField, $conditionValue,$conditionOperator);
                            $query = $query->condition($db_or);
                        }else{
                            $conditionField = $conditionArray[$i]->fieldName;
                            $conditionValue = $conditionArray[$i]->value;
                            if (isset($conditionArray[$i]->operator) && !empty($conditionArray[$i]->operator)){
                                $conditionOperator = $conditionArray[$i]->operator;
                            }else{
                                $conditionOperator = '=';
                            }
                            $db_and->condition($conditionField, $conditionValue,$conditionOperator);
                            $query = $query->condition($db_and);
                        }
                    }else{
                        $conditionField = $conditionArray[$i]->fieldName;
                        $conditionValue = $conditionArray[$i]->value;
                        if (isset($conditionArray[$i]->operator) && !empty($conditionArray[$i]->operator)){
                            $conditionOperator = $conditionArray[$i]->operator;
                        }else{
                            $conditionOperator = '=';
                        }
                        $db_and->condition($conditionField, $conditionValue,$conditionOperator);
                        $query = $query->condition($db_and);
                    }
                }
            }
            if (is_array($range) && !is_null($range)){
                $query = $query->range($range['min'], $range['max']);
            }
            $query = $query->execute()
                           ->fetchAll();
        }
    }
    return $query;
}

/**
 * @param null      $tableName
 * @param null      $userAccess
 * @param null      $fieldDataArray
 * @param null      $fieldPrimary
 * @param null      $fieldPrimaryValue
 * @param bool|true $useCreatedChanged
 *
 * @return null
 * @throws \Exception
 */
function saveTableData($tableName = null, $userAccess = null, $fieldDataArray = null, $fieldPrimary = null, $fieldPrimaryValue = null, $useCreatedChanged = true){
    if (!is_null($tableName) && !is_null($fieldDataArray) && !is_null($userAccess)){
        if (is_null($fieldPrimary) || is_null($fieldPrimaryValue)){
            //new data insert
            if (!(is_null($fieldPrimary) && is_null($fieldPrimaryValue))){
                $fieldDataArray[$fieldPrimary] = getRandomString();
                while (cek_duplicate_id($tableName, $fieldPrimary, $fieldDataArray[$fieldPrimary])){
                    $fieldDataArray[$fieldPrimary] = getRandomString();
                }
            }
            if ($useCreatedChanged && (!isset($fieldDataArray['created']) || !isset($fieldDataArray['changed']))){
                set_default_time_zone();
                $fieldDataArray['created'] = time();
                $fieldDataArray['changed'] = time();
            }
            $query = db_insert($tableName)
                ->fields($fieldDataArray)
                ->execute();
        }else{
            //update data
            $query = db_update($tableName)
                ->fields($fieldDataArray)
                ->condition($fieldPrimary, $fieldPrimaryValue)
                ->execute();
            $fieldDataArray[$fieldPrimary] = $fieldPrimaryValue;
        }
    }
    return $fieldDataArray;
}
function cek_duplicate_id($tableName = null, $idKey = null, $idValue = null){
    $duplicate = false;
    if (!empty($tableName) && !empty($idKey) && !empty($idValue)){
        $arrayValue = array(':id_value' => $idValue);
        $getExistingID = db_query('SELECT '.$idKey.' FROM {'.$tableName.'} WHERE '.$idKey.' = :id_value',$arrayValue)->fetchField();
        if (!empty($getExistingID)){
            $duplicate = true;
        }
    }
    return $duplicate;
}
/**
 * @param null $tableName
 * @param null $userAccess
 * @param null $conditionArray
 *
 * @return bool|\DatabaseStatementInterface
 */
function deleteTableData($tableName = null, $userAccess = null, $conditionArray = null){
    $query = false;
    if (!is_null($tableName) && !is_null($userAccess) && !is_null($conditionArray)){
        $queryCondition = db_and();
        for ($i = 0;$i < count($conditionArray);$i++){
            $conditionData = $conditionArray[$i];
            $operatorCondition = '=';
            if (isset($conditionData->fieldOperator) && !is_null($conditionData->fieldOperator)){
                $operatorCondition = $conditionData->fieldOperator;
            }
            if ($conditionData->status == 'or'){
                $queryCondition = $queryCondition->condition(db_or()->condition($conditionData->fieldName, $conditionData->fieldValue, $operatorCondition));
            }else{
                $queryCondition = $queryCondition->condition(db_and()->condition($conditionData->fieldName, $conditionData->fieldValue, $operatorCondition));
            }
        }
        if (user_access($userAccess)){
            $query = db_delete($tableName)
                ->condition($queryCondition)
                ->execute();
        }
    }
    return $query;
}

/**
 * @param int $length
 *
 * @return string
 */
function getRandomString($length=22)
{
    $key = '';
    $keys = array_merge(range(0, 9));
    for ($i = 0; $i < $length; $i++) {
        mt_srand((double)microtime() * 10000000);
        $key .= $keys[array_rand($keys)];
    }
    return $key;
}

/**
 * @return string
 */
function ikhwan_chart_sample(){
    $chart = array(
        '#type' => 'chart',
        '#chart_type' => 'line',
        '#chart_library' => 'highcharts', // Allowed values: 'google' or 'highcharts'
        '#title' => t('Simple Column Chart'),
    );
    $chart['female'] = array(
        '#type' => 'chart_data',
        '#title' => t('Female'),
        '#data' => array(12, 22, 32),
        '#suffix' => 'lbs',
    );
    $chart['xaxis'] = array(
        '#type' => 'chart_xaxis',
        '#labels' => array('Jan', 'Feb', 'Mar'),
    );
    $example['chart'] = $chart;

    return drupal_render($example);
}

/**
 * @param null $report_type
 * @param null $tahun
 * @param null $bulan
 * @return string
 */
function ikhwan_chart_keuangan($report_type = null, $tahun = null, $bulan = null){
    return 'Test';
}

/**
 * @param $mon
 * @param $year
 * @return mixed
 */
function get_last_day($mon, $year) {
    $daysinmonth = array(31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31);
    $days = $daysinmonth[$mon-1];
    if ($mon == 2 && ($year % 4) == 0 && (($year % 100) != 0 ||
            ($year % 400) == 0)) $days++;
    //if ($mon == 2 && ($year % 4) == 0 && ($year % 1000) != 0) $days++;
    $lastday = $days;
    return $lastday;
}

/**
 * @param null $kodeZone
 * @return null
 */
function getZoneByCode($kodeZone = null){
    $dataReturn = null;
    if (!empty($kodeZone)){
        $fieldArray = array(
            'zone', 'nama', 'alamat', 'telepon', 'whatsapp', 'bbm',
            'telegram', 'email', 'website', 'created', 'changed',
            'uid', 'uid_changed'
        );
        if (!is_array($kodeZone)){
            $dataZone = getTableDataByField('zdatazone','access ikhwan data zone',$fieldArray,null,'zone',$kodeZone);
            if (count($dataZone)){
                $dataReturn = $dataZone[0];
            }
        }else{
            $consData = new stdClass();
            $consData->fieldName = 'ta.zone';
            $consData->value = $kodeZone;
            $consData->operator = 'IN';
            $conditionArray[] = $consData;
            $dataZone = getTableDataByField(
                'zdatazone','access ikhwan data zone',$fieldArray,
                'zone',null,null,null,$conditionArray
            );
        }
    }else{
        $fieldArray = array(
            'zone', 'nama', 'alamat', 'telepon', 'whatsapp', 'bbm',
            'telegram', 'email', 'website', 'created', 'changed',
            'uid', 'uid_changed'
        );
        $dataZone = getTableDataByField('zdatazone','access ikhwan data zone',$fieldArray,'zone');
        if (count($dataZone)){
            $dataReturn = $dataZone;
        }
    }
    return $dataReturn;
}

/**
 * @return string
 */
function listZone($month = null, $year = null){
    if (!empty($month) && !empty($year)){
    }else{
        $month = date('n');
        $year = date('Y');
    }
    $intDate = mktime(7,0,0,$month,1,$year);
    $dataPendapatan = array();
    if (function_exists('get_total_pemasukan_pengeluaran_zon')){
        $dataPendapatan = get_total_pemasukan_pengeluaran_zon($month, $year);
    }
    $module_path = drupal_get_path('module','ikhwan_core');
    $csspath = $module_path.'/css/custom-style.css';
    drupal_add_css($csspath);
    $jspath = $module_path.'/js/list_zone.js';
    drupal_add_js($jspath,array('scope' => 'footer', 'weight' => 5));
    // Table settings.
    $attributes['datatable_options'] = array(
        'bFilter'   => TRUE,
        'bInfo'     => TRUE,
        'bPaginate' => FALSE,
    );
    // Define table columns
    $header = array(
        array(
            'data' => t('Kode'),
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
            'data' => t('Telephone'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Whatsapp'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Pendapatan').' ['.date('M',$intDate).']',
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
            'class' => 'col-input-number align-center',
        ),
        array(
            'data' => t('Pengeluaran').' ['.date('M',$intDate).']',
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
            'class' => 'col-input-number align-center',
        ),
        array(
            'data' => t('Baki').' ['.date('M',$intDate).']',
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
            'class' => 'col-input-number align-center',
        ),
        array(
            'data' => t('Edit'),
            'datatable_options' => array(
                'bSortable' => FALSE,
                'bSearchable' => FALSE,
            ),
        ),
        array(
            'data' => t('Entry'),
            'datatable_options' => array(
                'bSortable' => FALSE,
                'bSearchable' => FALSE,
            ),
        ),
    );
    // Table data.
    $dataZone = getZoneByCode();
    $rows = array();
    $totalPendapatan = 0;
    $totalPengeluaran = 0;
    $totalBaki = 0;
    for ($i = 0;$i < count($dataZone);$i++){
        $zoneData = array();
        $zoneData[] = $dataZone[$i]->zone;
        $zoneData[] = $dataZone[$i]->nama;
        //$zoneData[] = $dataZone[$i]->alamat;
        $zoneData[] = $dataZone[$i]->telepon;
        $zoneData[] = $dataZone[$i]->whatsapp;
        if (isset($dataPendapatan[$dataZone[$i]->zone])){
            $pendapatanZone = $dataPendapatan[$dataZone[$i]->zone]->total_pemasukan;
            $pendapatanZoneView = number_format(
                $dataPendapatan[$dataZone[$i]->zone]->total_pemasukan,2,'.',','
            );
            $pengeluaranZone = $dataPendapatan[$dataZone[$i]->zone]->total_pengeluaran;
            $pengeluaranZoneView = number_format(
                $pengeluaranZone, 2,'.',','
            );
            $bakiZone = $dataPendapatan[$dataZone[$i]->zone]->total_baki;
            $bakiZoneView = number_format(
                $bakiZone,2,'.',','
            );
        }else{
            $pendapatanZone = 0;
            $pendapatanZoneView = 0;
            $pengeluaranZone = 0;
            $pengeluaranZoneView = 0;
            $bakiZone = 0;
            $bakiZoneView = 0;
        }
        $totalPendapatan = $totalPendapatan + $pendapatanZone;
        $totalPengeluaran = $totalPengeluaran + $pengeluaranZone;
        $totalBaki = $totalBaki + $bakiZone;
        $zoneData[] = array('data' => $pendapatanZoneView, 'class' => 'align-right');
        $zoneData[] = array('data' => $pengeluaranZoneView, 'class' => 'align-right');
        $zoneData[] = array('data' => $bakiZoneView, 'class' => 'align-right');
        $editIcon = '<i class="fa fa-edit"></i>';
        $editButtonVar = array(
            'element' => array(
                '#attributes' => array(
                    'class' => array('btn-danger','btn-xs','no-margin','edit-zone'),
                    'id' => 'edit-zone-'.$dataZone[$i]->zone,
                    'name' => 'edit-zone-'.$dataZone[$i]->zone,
                ),
                '#button_type' => 'button',
                '#value' => $editIcon.' EDIT',
            )
        );
        $editButton = theme('button', $editButtonVar);
        $zoneData[] = $editButton;
        $addIcon = '<i class="fa fa-plus"></i>';
        $addButtonVar = array(
            'element' => array(
                '#attributes' => array(
                    'class' => array('btn-primary','btn-xs','no-margin','add-pendapatan'),
                    'id' => 'add-pendapatan-'.$dataZone[$i]->zone,
                    'name' => 'add-pendapatan-'.$dataZone[$i]->zone,
                    'onclick' => 'entry_pemasukan(\''.$dataZone[$i]->zone.'\', '.$month.','.$year.')',
                ),
                '#button_type' => 'button',
                '#value' => $editIcon.' ENTRY KEWANGAN',
            )
        );
        $addButton = theme('button', $addButtonVar);
        $zoneData[] = $addButton;
        $rows[] = $zoneData;
    }
    // Or, render using a theme function.
    $footer[] = array(
        array(
            'data' => '<strong>'.t('TOTAL').'</strong>',
            'colspan' => 4,
            'class' => 'align-right',
        ),
        array(
            'data' => '<strong>'.number_format($totalPendapatan,2,'.',',').'</strong>',
            'class' => 'col-input-number align-right',
        ),
        array(
            'data' => '<strong>'.number_format($totalPengeluaran,2,'.',',').'</strong>',
            'class' => 'col-input-number align-right',
        ),
        array(
            'data' => '<strong>'.number_format($totalBaki,2,'.',',').'</strong>',
            'class' => 'col-input-number align-right',
        ),
        array(
            'data' => '&nbsp;',
            'colspan' => 2,
        ),
    );
    $variables = array(
        'attributes' => $attributes,
        'header' => $header,
        'rows' => $rows,
        'footer' => $footer,
    );
    $newZoneButtonVar = array(
        'element' => array(
            '#attributes' => array(
                'class' => array('btn-info btn-sm'),
                'id' => 'new-zone',
                'name' => 'new-zone',
            ),
            '#button_type' => 'button',
            '#value' => t('Tambah Data Zone'),
        )
    );
    $newZoneButton = theme('button', $newZoneButtonVar);
    $variables['content'] = t('Pilih Bulan dan Tahun Pendapatan dan Pengeluaran, kemudian klik tombol show untuk melihat Pendapatan dan Pengeluaran');
    $variables['month'] = $month;
    $variables['year'] = $year;
    $form = drupal_get_form('filter_periode_form',$variables);
    $formPendapatan = drupal_render($form);
    $listZone = $newZoneButton.$formPendapatan.theme('datatable', $variables);
    return $listZone;
}

/**
 * @param null $kodeZone
 * @return string
 */
function formZone($kodeZone = null){
    $module_path = drupal_get_path('module','ikhwan_core');
    $csspath = $module_path.'/css/custom-style.css';
    drupal_add_css($csspath);
    $getDataZone = null;
    if (!empty($kodeZone)){
        $getDataZone = getZoneByCode($kodeZone);
    }
    if (empty($getDataZone)){
        $getDataZone = new stdClass();
        $getDataZone->zone = '';
        $getDataZone->nama = '';
        $getDataZone->alamat = '';
        $getDataZone->telepon = '';
        $getDataZone->whatsapp = '';
        $getDataZone->bbm = '';
        $getDataZone->telegram = '';
        $getDataZone->email = '';
        $getDataZone->website = '';
    }
    $variables = $getDataZone;
    $form = drupal_get_form('formZone_form',$variables);
    $formZone = drupal_render($form);
    return $formZone;
}

/**
 * @param $form
 * @param $form_state
 * @param null $variables
 * @return mixed
 */
function formZone_form($form, &$form_state, $variables = null){
    $form['#attributes'] = array('class' => array('col-sm-12'));
    $form['basic'] = array(
        '#type' => 'fieldset',
        '#title' => t('Zone Info'),
        '#collapsible' => TRUE, // Added
        '#collapsed' => FALSE,  // Added
        '#prefix' => '<div class="col-md-6">',
    );
    if (!empty($variables->zone)){
        $form['basic']['zone_code'] = array(
            '#type' => 'hidden',
            '#default_value' => $variables->zone,
        );
    }
    $form['basic']['zone'] = array(
        '#type' => 'textfield',
        '#title' => t('Kode Zone'),
        '#required' => TRUE,
        '#default_value' => $variables->zone,
    );
    $form['basic']['nama'] = array(
        '#type' => 'textfield',
        '#title' => t('Nama Zone'),
        '#required' => TRUE,
        '#default_value' => $variables->nama,
    );
    $form['basic']['alamat'] = array(
        '#title' => t('Alamat'),
        '#type' => 'textarea',
        '#default_value' => $variables->alamat,
    );
    $form['basic']['website'] = array(
        '#type' => 'textfield',
        '#title' => t('Website'),
        '#default_value' => $variables->website,
    );
    $form['contact'] = array(
        '#type' => 'fieldset',
        '#title' => t('Zone Contact'),
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
        '#value' => t('Save Data Zone'),
        '#type' => 'submit',
        '#attributes' => array('class' => array('btn btn-primary')),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>',
    );
    return $form;
}

/**
 * @param $form
 * @param $form_state
 */
function formZone_form_submit($form, &$form_state) {
    $values = $form_state['values'];
    $savedData = array();
    $savedData['zone'] = $values['zone'];
    $savedData['nama'] = $values['nama'];
    $savedData['alamat'] = $values['alamat'];
    $savedData['website'] = $values['website'];
    $savedData['telepon'] = $values['telepon'];
    $savedData['whatsapp'] = $values['whatsapp'];
    $savedData['bbm'] = $values['bbm'];
    $savedData['telegram'] = $values['telegram'];
    $savedData['email'] = $values['email'];
    if (isset($values['zone_code']) && !empty($values['zone_code'])){
        $updatedData = saveTableData('zdatazone','admin ikhwan data zone',$savedData,'zone',$values['zone_code']);
        $returnData = $updatedData;
        drupal_set_message("Data zone berhasil di update");
        drupal_goto('masterdata/zone');
    }else{
        $insertedData = saveTableData('zdatazone','admin ikhwan data zone',$savedData);
        $returnData = $insertedData;
        drupal_set_message("Data zone berhasil di simpan");
    }
    return $returnData;
}

/**
 * @return string
 */
function listAssigment(){
    $module_path = drupal_get_path('module','ikhwan_core');
    $csspath = $module_path.'/css/custom-style.css';
    drupal_add_css($csspath);
    $jspath = $module_path.'/js/list_assignment.js';
    drupal_add_js($jspath,array('scope' => 'footer', 'weight' => 5));
    // Table settings.
    $attributes['datatable_options'] = array(
        'bFilter'   => TRUE,
        'bInfo'     => TRUE,
    );
    // Define table columns
    $header = array(
        array(
            'data' => t('Kode Assigment'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Nama Assigment'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Periode'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Judul Periode'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Weight'),
            'datatable_options' => array(
                'bSortable' => TRUE,
                'bSearchable' => TRUE,
            ),
        ),
        array(
            'data' => t('Edit'),
            'datatable_options' => array(
                'bSortable' => FALSE,
                'bSearchable' => FALSE,
            ),
        ),
    );
    // Table data.
    $dataAssignment = get_assignment_by_id();
    $rows = array();
    $periodeArray = create_periode_option();
    for ($i = 0;$i < count($dataAssignment);$i++){
        $assignmentData = array();
        $assignmentData[] = $dataAssignment[$i]->assignment_code;
        $assignmentData[] = $dataAssignment[$i]->assignment_name;
        $assignmentData[] = $periodeArray[$dataAssignment[$i]->periode];
        $assignmentData[] = $dataAssignment[$i]->judul_periode;
        $assignmentData[] = $dataAssignment[$i]->weight;
        $editIcon = '<i class="fa fa-edit"></i>';
        $editButtonVar = array(
            'element' => array(
                '#attributes' => array(
                    'class' => array('btn-danger','btn-xs','no-margin','edit-assignment'),
                    'id' => 'edit-assignment-'.$dataAssignment[$i]->id,
                    'name' => 'edit-assignment-'.$dataAssignment[$i]->id,
                ),
                '#button_type' => 'button',
                '#value' => $editIcon.' EDIT',
            )
        );
        $editButton = theme('button', $editButtonVar);
        $assignmentData[] = $editButton;
        $rows[] = $assignmentData;
    }
    // Or, render using a theme function.
    $variables = array(
        'attributes' => $attributes,
        'header' => $header,
        'rows' => $rows,
    );
    $newAssignmentButtonVar = array(
        'element' => array(
            '#attributes' => array(
                'class' => array('btn-info btn-sm'),
                'id' => 'new-assignment',
                'name' => 'new-assignment',
            ),
            '#button_type' => 'button',
            '#value' => t('Tambah Data Assignment'),
        )
    );
    $newAssignmentButton = theme('button', $newAssignmentButtonVar);
    $listAssigment = $newAssignmentButton.theme('datatable', $variables);
    return $listAssigment;
}

/**
 * @param null $assignmentId
 * @return $this|null|SelectQuery|SelectQueryInterface
 */
function get_assignment_by_id($assignmentId = null){
    $dataReturn = null;
    $fieldArray = array(
        'id', 'assignment_code', 'assignment_name', 'judul_periode', 'periode',
        'created', 'changed', 'uid', 'uid_changed','weight'
    );
    if (!empty($assignmentId)){
        $dataAssignment = getTableDataByField(
            'zdata_assigment','access ikhwan data assignment',$fieldArray,'weight','id',$assignmentId
        );
        if (count($dataAssignment)){
            $dataReturn = $dataAssignment[0];
        }
    }else{
        $dataAssignment = getTableDataByField(
            'zdata_assigment','access ikhwan data assignment',$fieldArray,'weight'
        );
        if (count($dataAssignment)){
            $dataReturn = $dataAssignment;
        }
    }
    return $dataReturn;
}

/**
 * @return string
 */
function formInputJenisAssigment($idAssignment = null){
    $getDataAssignment = null;
    if (!empty($idAssignment)){
        $getDataAssignment = get_assignment_by_id($idAssignment);
    }
    if (empty($getDataAssignment)){
        $getDataAssignment = new stdClass();
        $getDataAssignment->id = '';
        $getDataAssignment->assignment_code = '';
        $getDataAssignment->assignment_name = '';
        $getDataAssignment->judul_periode = '';
        $getDataAssignment->periode = 0;
        $getDataAssignment->weight = 0;
    }
    $variables = $getDataAssignment;
    $form = drupal_get_form('formJenisAssignment_form',$variables);
    $formAssignment = drupal_render($form);
    return $formAssignment;
}

/**
 * @return array
 */
function create_periode_option(){
    return array(
        0 => 'Harian',
        1 => 'Mingguan',
        2 => 'Bulanan',
        3 => 'Tahunan',
        4 => 'Insidentil',
    );
}

/**
 * @param $form
 * @param $form_state
 * @param null $variables
 */
function formJenisAssignment_form($form, &$form_state, $variables = null){
    $form['#attributes'] = array('class' => array('col-md-12'));
    $form['basic'] = array(
        '#type' => 'fieldset',
        '#title' => t('Master Data Assignment'),
        '#collapsible' => TRUE, // Added
        '#collapsed' => FALSE,  // Added
        '#prefix' => '<div class="col-md-6">',
    );
    if (!empty($variables->id)){
        $form['basic']['id'] = array(
            '#type' => 'hidden',
            '#default_value' => $variables->id,
        );
    }
    $form['basic']['assignment_code'] = array(
        '#type' => 'textfield',
        '#title' => t('Kode Assignment'),
        '#required' => TRUE,
        '#default_value' => $variables->assignment_code,
    );
    $form['basic']['assignment_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Nama Assignment'),
        '#required' => TRUE,
        '#default_value' => $variables->assignment_name,
    );
    $periodeOptions = create_periode_option();
    $form['basic']['periode'] = array(
        '#title' => t('Periode Assignment'),
        '#type' => 'select',
        '#required' => TRUE,
        '#options' => $periodeOptions,
        '#default_value' => $variables->periode,
        '#select2' => array(
            'placeholder' => 'Placeholder text',
            'width' => '150',
        ),
    );
    $form['basic']['judul_periode'] = array(
        '#type' => 'textfield',
        '#title' => t('Judul Periode'),
        '#default_value' => $variables->judul_periode,
    );
    $form['basic']['weight'] = array(
        '#type' => 'textfield',
        '#title' => t('Weight'),
        '#default_value' => $variables->weight,
        '#suffix' => '</div>',
    );
    $form['saveButton'] = array(
        '#value' => t('Save Data Assignment'),
        '#type' => 'submit',
        '#attributes' => array('class' => array('btn btn-primary')),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>',
    );
    return $form;
}

/**
 * @param $form
 * @param $form_state
 * @return null
 */
function formJenisAssignment_form_submit($form, &$form_state) {
    global $user;
    $values = $form_state['values'];
    $savedData = array();
    $savedData['assignment_code'] = $values['assignment_code'];
    $savedData['assignment_name'] = $values['assignment_name'];
    $savedData['judul_periode'] = $values['judul_periode'];
    $savedData['periode'] = $values['periode'];
    $savedData['weight'] = $values['weight'];
    if (isset($values['id']) && !empty($values['id'])){
        $savedData['uid_changed'] = $user->uid;
        $updatedData = saveTableData('zdata_assigment','admin ikhwan data assignment',$savedData,'id',$values['id']);
        $returnData = $updatedData;
        drupal_set_message("Data assignment berhasil di update");
        drupal_goto('masterdata/assignment');
    }else{
        $savedData['uid'] = $user->uid;
        $insertedData = saveTableData('zdata_assigment','admin ikhwan data assignment',$savedData,'id');
        $returnData = $insertedData;
        drupal_set_message("Data assignment berhasil di simpan");
    }
    return $returnData;
}

/**
 * @param int $assignmentType
 * @return array
 */
function create_assignment_header($assignmentType = 0, $offset = 0, $month = null, $year = null){
    set_default_time_zone();
    $tableHeader = array();
    $tableHeader[] = t('BIL.');
    $tableHeader[] = t('ZON');
    if (!empty($month) && !empty($year)){
    }else{
        $month = date('n');
        $year = date('Y');
    }
    if ($assignmentType == 0){
        $lastDate = get_last_day($month,$year);
        $intDate = mktime(7,0,0,$month,1,$year);
        $startDate = date('Y-m-',$intDate).'01';
        $finishDate = date('Y-m-',$intDate).$lastDate;
        $dateArray = create_date_range_array($startDate,$finishDate, 'd-M');
        if ($offset == 0){
            $firstData = 0;
            $nextData = 10;
        }else if($offset == 1){
            $firstData = 10;
            $nextData = 20;
        }else if($offset == 2){
            $firstData = 20;
            $nextData = count($dateArray);
        }
        for ($i = $firstData;$i < $nextData;$i++){
            $tableHeader[] = array('data' => $dateArray[$i],'class' => 'center');
        }
    }else if($assignmentType == 1){
        for ($i = 0;$i < 5;$i++){
            $tableHeader[] = t('Minggu ').($i + 1);
        }
    }else{
        $tableHeader[] = t('JUMLAH');
    }
    return $tableHeader;
}
function create_assignment_footer($assignmentType = 0, $footerValue = null, $offset = 0, $month = null, $year = null){
    set_default_time_zone();
    $tableFooter = array();
    $tableFooter[] = '';
    $tableFooter[] = '<strong>'.t('TOTAL').'</strong>';
    if (!empty($month) && !empty($year)){
    }else{
        $month = date('n');
        $year = date('Y');
    }
    if ($assignmentType == 0){
        $lastDate = get_last_day($month,$year);
        $intDate = mktime(7,0,0,$month,1,$year);
        $startDate = date('Y-m-',$intDate).'01';
        $finishDate = date('Y-m-',$intDate).$lastDate;
        $dateArray = create_date_range_array($startDate,$finishDate, 'Y-m-d');
        if ($offset == 0){
            $firstData = 0;
            $nextData = 10;
        }else if($offset == 1){
            $firstData = 10;
            $nextData = 20;
        }else if($offset == 2){
            $firstData = 20;
            $nextData = count($dateArray);
        }
        for ($i = $firstData;$i < $nextData;$i++){
            $explodeDate = explode('-',$dateArray[$i]);
            $idxDate = mktime(0,0,0,$explodeDate[1],$explodeDate[2],$explodeDate[0]);
            $footerData = isset($footerValue[$idxDate]) ? $footerValue[$idxDate] : 0;
            if (is_numeric($footerData)){
                $footerData = '<strong>'.number_format($footerData,0,'.',',').'</strong>';
            }
            $tableFooter[] = array(
                'data' => $footerData,
                'class' => array(
                    'align-right',
                ),
                'id' => 'foot-'.$idxDate,
            );
        }
    }else if($assignmentType == 1){
        for ($i = 0;$i < 5;$i++){
            $idxDate = $i + 1;
            $footerData = isset($footerValue[$idxDate]) ? $footerValue[$idxDate] : 0;
            if (is_numeric($footerData)){
                $footerData = '<strong>'.number_format($footerData,0,'.',',').'</strong>';
            }
            $tableFooter[] = array(
                'data' => $footerData,
                'class' => array(
                    'align-right',
                ),
                'id' => 'foot-'.$idxDate,
            );
        }
    }else{
        $footerData = isset($footerValue[1]) ? $footerValue[1] : 0;
        if (is_numeric($footerData)){
            $footerData = '<strong>'.number_format($footerData,0,'.',',').'</strong>';
        }
        $tableFooter[] = array(
            'data' => $footerData,
            'class' => array(
                'align-right',
            ),
            'id' => 'foot-1',
        );
    }
    return array($tableFooter);
}
/**
 * @param int $assignmentType
 * @param null $assigmentZone
 * @return array
 */
function create_assignment_row($assignmentType = 0, $assigmentZone = null, $offset = 0, $month = null, $year = null){
    set_default_time_zone();
    $dataZone = getZoneByCode();
    $tableRow = array();
    if (!empty($month) && !empty($year)){
    }else{
        $month = date('n');
        $year = date('Y');
    }
    if ($assignmentType == 0){
        $lastDate = get_last_day($month,$year);
        $intDate = mktime(7,0,0,$month,1,$year);
        $startDate = date('Y-m-',$intDate).'01';
        $finishDate = date('Y-m-',$intDate).$lastDate;
        $dateArray = create_date_range_array($startDate,$finishDate, 'Y-m-d');
        $newDateArray = array();
        $tableRow['#tree'] = TRUE;
        for ($i = 0;$i < count($dateArray);$i++){
            $splitDate = explode('-',$dateArray[$i]);
            $newDate = mktime(0,0,0,$splitDate[1],$splitDate[2],$splitDate[0]);
            $newDateArray[] = $newDate;
        }
        $totalColumn = array();
        for ($i = 0;$i < count($dataZone);$i++){
            $rowData = array();
            $rowData['b-'.$dataZone[$i]->zone] = array(
                '#type' => 'item', '#title' => ($i + 1),
                '#attributes' => array('outerclass' => array('align-right')),
            );
            $rowData['c-'.$dataZone[$i]->zone] = array('#type' => 'item', '#title' => $dataZone[$i]->nama);
            if ($offset == 0){
                $firstData = 0;
                $nextData = 10;
            }else if($offset == 1){
                $firstData = 10;
                $nextData = 20;
            }else if($offset == 2){
                $firstData = 20;
                $nextData = count($newDateArray);
            }
            for ($j = $firstData;$j < $nextData;$j++){
                if (isset($assigmentZone[$dataZone[$i]->zone][$year][$month][$newDateArray[$j]])){
                    $idAssignmentZone = $assigmentZone[$dataZone[$i]->zone][$year][$month][$newDateArray[$j]]->id;
                    $bankIn = $assigmentZone[$dataZone[$i]->zone][$year][$month][$newDateArray[$j]]->bank_in;
                    $rowData['c-'.$dataZone[$i]->zone.'-'.$newDateArray[$j].'-'.$idAssignmentZone] = array(
                        '#type' => 'textfield',
                        '#default_value' => $bankIn,
                        '#attributes' => array('class' => array(
                            'align-right','assignment-value',
                            'assignment-column-'.$newDateArray[$j],
                            ),
                        ),
                    );
                    if (!isset($totalColumn[$newDateArray[$j]])){
                        $totalColumn[$newDateArray[$j]] = $bankIn;
                    }else{
                        $totalColumn[$newDateArray[$j]] += $bankIn;
                    }
                }else{
                    $rowData['c-'.$dataZone[$i]->zone.'-'.$newDateArray[$j]] = array(
                        '#type' => 'textfield',
                        '#attributes' => array(
                            'class' => array(
                                'align-right','assignment-value',
                                'assignment-column-'.$newDateArray[$j],
                            ),
                        ),
                    );
                }
            }
            $tableRow['r'.($i + 1)] = $rowData;
        }
    }else if($assignmentType == 1){
        $totalColumn = array();
        for ($i = 0;$i < count($dataZone);$i++) {
            $rowData = array();
            $rowData['b-'.$dataZone[$i]->zone] = array(
                '#type' => 'item', '#title' => ($i + 1),
                '#attributes' => array('outerclass' => array('align-right')),
            );
            $rowData['c-'.$dataZone[$i]->zone] = array('#type' => 'item', '#title' => $dataZone[$i]->nama);
            for ($j = 0; $j < 5; $j++) {
                if (isset($assigmentZone[$dataZone[$i]->zone][$year][$month][($j + 1)])){
                    $idAssignmentZone = $assigmentZone[$dataZone[$i]->zone][$year][$month][($j + 1)]->id;
                    $bankIn = $assigmentZone[$dataZone[$i]->zone][$year][$month][($j + 1)]->bank_in;
                    $rowData['c-'.$dataZone[$i]->zone.'-'.($j + 1).'-'.$idAssignmentZone] = array(
                        '#type' => 'textfield',
                        '#default_value' => $bankIn,
                        '#attributes' => array(
                            'class' => array(
                                'align-right','assignment-value',
                                'assignment-column-'.($j + 1),
                            ),
                        ),
                    );
                    if (!isset($totalColumn[($j + 1)])){
                        $totalColumn[($j + 1)] = $bankIn;
                    }else{
                        $totalColumn[($j + 1)] += $bankIn;
                    }
                }else{
                    $rowData['c-'.$dataZone[$i]->zone.'-'.($j + 1)] = array(
                        '#type' => 'textfield',
                        '#attributes' => array(
                            'class' => array(
                                'align-right','assignment-value',
                                'assignment-column-'.($j + 1),
                            ),
                        ),
                    );
                }
            }
            $tableRow['r'.($i + 1)] = $rowData;
        }
    }else{
        $totalColumn = array();
        for ($i = 0;$i < count($dataZone);$i++) {
            $rowData = array();
            $rowData['b-'.$dataZone[$i]->zone] = array(
                '#type' => 'item', '#title' => ($i + 1),
                '#attributes' => array('outerclass' => array('align-right')),
            );
            $rowData['c-'.$dataZone[$i]->zone] = array('#type' => 'item', '#title' => $dataZone[$i]->nama);
            for ($j = 0; $j < 1; $j++) {
                if (isset($assigmentZone[$dataZone[$i]->zone][$year][$month][($j + 1)])){
                    $idAssignmentZone = $assigmentZone[$dataZone[$i]->zone][$year][$month][($j + 1)]->id;
                    $bankIn = $assigmentZone[$dataZone[$i]->zone][$year][$month][($j + 1)]->bank_in;
                    $rowData['c-'.$dataZone[$i]->zone.'-'.($j + 1).'-'.$idAssignmentZone] = array(
                        '#type' => 'textfield',
                        '#default_value' => $bankIn,
                        '#attributes' => array(
                            'class' => array(
                                'align-right','assignment-value',
                                'assignment-column-'.($j + 1),
                            ),
                        ),
                    );
                    if (!isset($totalColumn[($j + 1)])){
                        $totalColumn[($j + 1)] = $bankIn;
                    }else{
                        $totalColumn[($j + 1)] += $bankIn;
                    }
                }else{
                    $rowData['c-'.$dataZone[$i]->zone.'-'.($j + 1)] = array(
                        '#type' => 'textfield',
                        '#attributes' => array(
                            'class' => array(
                                'align-right','assignment-value',
                                'assignment-column-'.($j + 1),
                            ),
                        ),
                    );
                }
            }
            $tableRow['r'.($i + 1)] = $rowData;
        }
    }
    return array($tableRow,$totalColumn);
}
/**
 * @param $strDateFrom
 * @param $strDateTo
 * @param string $dateFormat
 * @return array
 */
function create_date_range_array($strDateFrom, $strDateTo, $dateFormat = 'Y-m-d')
{
    set_default_time_zone();
    $aryRange=array();
    $iDateFrom=mktime(0,0,0,substr($strDateFrom,5,2),     substr($strDateFrom,8,2),substr($strDateFrom,0,4));
    $iDateTo=mktime(0,0,0,substr($strDateTo,5,2),     substr($strDateTo,8,2),substr($strDateTo,0,4));

    if ($iDateTo>=$iDateFrom)
    {
        if ($dateFormat == 'UNIX'){
            array_push($aryRange,$iDateFrom); // first entry
        }else{
            array_push($aryRange,date($dateFormat,$iDateFrom)); // first entry
        }
        while ($iDateFrom<$iDateTo)
        {
            $iDateFrom+=86400; // add 24 hours
            if ($dateFormat == 'UNIX'){
                array_push($aryRange,$iDateFrom);
            }else {
                array_push($aryRange, date($dateFormat, $iDateFrom));
            }
        }
    }
    return $aryRange;
}

/**
 * @param null $assigmentId
 * @return string
 * @throws Exception
 */
function assigmentZone($assigmentId = null, $offset = 0, $month = null, $year = null){
    if (!empty($assigmentId)){
        drupal_add_js(
            array(
                'assigmentId' => $assigmentId,
                'bulan' => $month,
                'tahun' => $year,
            )
            , 'setting');
    }
    $module_path = drupal_get_path('module','ikhwan_core');
    $csspath = $module_path.'/css/custom-style.css';
    drupal_add_css($csspath);
    $jspath = $module_path.'/js/assignment_zone.js';
    drupal_add_js($jspath,array('scope' => 'footer', 'weight' => 5));
    $jenisAssignment = get_assignment_by_id();
    $assignmentZone = '<div class="col-md-12">';
    for ($i = 0;$i < count($jenisAssignment);$i++){
        $assignmentButtonVar = array(
            'element' => array(
                '#attributes' => array(
                    'class' => array('btn-primary btn-sm input-assignment'),
                    'id' => 'assignment-'.$jenisAssignment[$i]->id,
                    'name' => 'assignment-'.$jenisAssignment[$i]->id,
                ),
                '#button_type' => 'button',
                '#value' => $jenisAssignment[$i]->assignment_name,
            )
        );
        $newAssignmentButton = theme('button', $assignmentButtonVar);
        $assignmentZone .= $newAssignmentButton;
    }
    $assignmentZone .= '</div>';
    $assignmentZone .= '<div class="col-md-12">';
    if (!empty($assigmentId)){
        $variables['id'] = $assigmentId;
        $variables['month'] = $month;
        $variables['year'] = $year;
        $jenisAssignment = get_assignment_by_id($variables['id']);
        drupal_add_js(
            array(
                'jenisAssignment' => $jenisAssignment,
            )
        , 'setting');
        $dateButton = '';
        if ($jenisAssignment->periode == 0){
            if ($offset == 0){
                $btnClass = 'btn-primary';
            }else{
                $btnClass = 'btn-default';
            }
            $inputDateButtonVar = array(
                'element' => array(
                    '#attributes' => array(
                        'class' => array('btn-sm','input-offset',$btnClass,),
                        'id' => 'offset-0',
                        'name' => 'offset-0',
                    ),
                    '#button_type' => 'button',
                    '#value' => t('Tanggal 1-10'),
                )
            );
            if ($offset == 1){
                $btnClass = 'btn-primary';
            }else{
                $btnClass = 'btn-default';
            }
            $inputDateButton = theme('button', $inputDateButtonVar);
            $dateButton .= $inputDateButton;
            $inputDateButtonVar = array(
                'element' => array(
                    '#attributes' => array(
                        'class' => array('btn-sm','input-offset',$btnClass,),
                        'id' => 'offset-1',
                        'name' => 'offset-1',
                    ),
                    '#button_type' => 'button',
                    '#value' => t('Tanggal 11-20'),
                )
            );
            $inputDateButton = theme('button', $inputDateButtonVar);
            $dateButton .= $inputDateButton;
            if ($offset == 2){
                $btnClass = 'btn-primary';
            }else{
                $btnClass = 'btn-default';
            }
            $lastDate = get_last_day($month,$year);
            $inputDateButtonVar = array(
                'element' => array(
                    '#attributes' => array(
                        'class' => array('btn-sm','input-offset',$btnClass,),
                        'id' => 'offset-2',
                        'name' => 'offset-2',
                    ),
                    '#button_type' => 'button',
                    '#value' => t('Tanggal 20-'.$lastDate),
                )
            );
            $inputDateButton = theme('button', $inputDateButtonVar);
            $dateButton .= $inputDateButton;
        }
        $variables['offset'] = $offset;
        $form = drupal_get_form('assignment_zone_form',$variables);
        $formAssignment = drupal_render($form);
        $assignmentZone .= $dateButton.$formAssignment;
    }else{
        $variables['content'] = t('Pilih Bulan dan Tahun Assigment, kemudian klik tombol 
        assigment diatas untuk memulai entry assignment');
        $form = drupal_get_form('infoForm',$variables);
        $formAssignment = drupal_render($form);
        $assignmentZone .= $formAssignment;
    }
    $assignmentZone .= '</div>';
    return $assignmentZone;
}

/**
 * @param $form
 * @param $form_state
 * @param null $variables
 * @return mixed
 */
function infoForm($form, &$form_state, $variables = null){
    if (!isset($variables['content'])) {
        $variables['content'] = t('Pilih Bulan dan Tahun Assigment, kemudian klik tombol 
        assigment diatas untuk memulai entry assignment'
        );
    }
    $monthArray = array(
        1 => 'Januari',
        2 => 'Februari',
        3 => 'Maret',
        4 => 'April',
        5 => 'Mei',
        6 => 'Juni',
        7 => 'Juli',
        8 => 'Agustus',
        9 => 'September',
        10 => 'Oktober',
        11 => 'November',
        12 => 'Desember',
    );
    $form['basic'] = array(
        '#type' => 'fieldset',
        '#title' => t('Form Entry Assignment Zon'),
        '#collapsible' => FALSE, // Added
        '#collapsed' => FALSE,  // Added
        '#attributes' => array('class' => array('form-inline')),
    );
    $form['basic']['info_content'] = array(
        '#type' => 'item',
        '#markup' => $variables['content'],
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>',
    );
    $currDate = date('j');
    if ($currDate < 10){
        $intDate = mktime(7,0,0,(date('n') - 1),1,date('Y'));
        $month = date('n',$intDate);
        $year = date('Y',$intDate);
    }else{
        $month = date('n');
        $year = date('Y');
    }
    $form['basic']['month'] = array(
        '#type' => 'select',
        '#title' => t('Bulan'),
        '#default_value' => $month,
        '#options' => $monthArray,
        '#select2' => array(
            'placeholder' => 'Pilih Bulan Assignment',
        ),
    );
    $form['basic']['year'] = array(
        '#type' => 'textfield',
        '#title' => t('Tahun'),
        '#default_value' => $year,
        '#attributes' => array('class' => array('col-number')),
    );
    return $form;
}

function filter_periode_form($form, &$form_state, $variables = null){
    if (!isset($variables['content'])) {
        $variables['content'] = t('Pilih Bulan dan Tahun Assigment, kemudian klik tombol 
        assigment diatas untuk memulai entry assignment'
        );
    }
    $monthArray = array(
        1 => 'Januari',
        2 => 'Februari',
        3 => 'Maret',
        4 => 'April',
        5 => 'Mei',
        6 => 'Juni',
        7 => 'Juli',
        8 => 'Agustus',
        9 => 'September',
        10 => 'Oktober',
        11 => 'November',
        12 => 'Desember',
    );
    $form['info_content'] = array(
        '#type' => 'item',
        '#markup' => $variables['content'],
        '#prefix' => '<div>',
        '#suffix' => '</div>',
    );
    $currDate = date('j');
    if (isset($variables['month']) && isset($variables['year'])){
        $month = $variables['month'];
        $year = $variables['year'];
    }else {
        if ($currDate < 10) {
            $intDate = mktime(7, 0, 0, (date('n') - 1), 1, date('Y'));
            $month = date('n', $intDate);
            $year = date('Y', $intDate);
        } else {
            $month = date('n');
            $year = date('Y');
        }
    }
    if (isset($variables['alamat']) && !empty($variables['alamat'])){
        $alamat = $variables['alamat'];
    }else{
        $alamat = 'masterdata/zone';
    }
    $form['#attributes'] = array('class' => array('form-inline'));
    $form['month'] = array(
        '#type' => 'select',
        '#title' => t('Bulan'),
        '#default_value' => $month,
        '#options' => $monthArray,
        '#select2' => array(
            'placeholder' => 'Pilih Bulan Assignment',
        ),
    );
    $form['year'] = array(
        '#type' => 'textfield',
        '#title' => t('Tahun'),
        '#default_value' => $year,
        '#attributes' => array('class' => array('col-number')),
    );
    $form['alamat'] = array(
        '#type' => 'hidden',
        '#default_value' => $alamat,
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Show'),
        '#attributes' => array('class' => array('btn-sm btn-warning'), 'style' => 'margin: -8px 10px 0 !important;'),
    );
    return $form;
}

function filter_periode_form_submit($form, &$form_state){
    global $user;
    set_default_time_zone();
    $values = $form_state['values'];
    drupal_goto($values['alamat'].'/'.$values['month'].'/'.$values['year']);
}
/**
 * @param $form
 * @param $form_state
 * @param null $variables
 * @return mixed
 */
function assignment_zone_form($form, &$form_state, $variables = null){
    set_default_time_zone();
    if (isset($variables['id']) && !empty($variables['id'])){
        $jenisAssignment = get_assignment_by_id($variables['id']);
        if (isset($variables['offset'])){
            $offset = $variables['offset'];
        }else{
            $offset = 0;
        }
        if (isset($variables['month']) && isset($variables['year'])){
            $hiddenMonth = $variables['month'];
            $hiddenYear = $variables['year'];
        }else{
            $variables['month'] = null;
            $variables['year'] = null;
            $hiddenMonth = '0';
            $hiddenYear = '0';
        }
        $intDate = mktime(7,0,0,$variables['month'],1,$variables['year']);
        $tableHeader = create_assignment_header($jenisAssignment->periode,$offset,$variables['month'],$variables['year']);
        $assignmentZone = get_assignment_zone_data($variables['id'],false,null,$variables['month'],$variables['year']);
        $tableRow = create_assignment_row($jenisAssignment->periode,$assignmentZone,$offset,$variables['month'],$variables['year']);
        $tableFooter = create_assignment_footer($jenisAssignment->periode,$tableRow[1],$offset,$variables['month'],$variables['year']);
        if ($jenisAssignment->periode > 1){
            $form['#attributes'] = array('class' => array('col-md-6'));
        }
        $form['form-title'] = array(
            '#type' => 'item',
            '#markup' => '<h3 class="title">'.$jenisAssignment->assignment_name.' '.date('M',$intDate).' '.date('Y',$intDate).'</h3>',
        );
        $form['assignment_id'] = array(
            '#type' => 'hidden',
            '#default_value' => $variables['id'],
        );
        $form['month'] = array(
            '#type' => 'hidden',
            '#default_value' => $hiddenMonth,
            '#attributes' => array('id' => 'edit-month',),
        );
        $form['year'] = array(
            '#type' => 'hidden',
            '#default_value' => $hiddenYear,
            '#attributes' => array('id' => 'edit-year',),
        );
        $form['table'] = array(
            '#theme' => 'formtable_form_table',
            '#header' => $tableHeader,
            'rows' => $tableRow[0],
            '#footer' => $tableFooter,
        );
        $form['form-grand'] = array(
            '#type' => 'item',
            '#markup' => '<h4 id="grand-total" class="title"><strong>GRAND TOTAL : 0</strong></h4>',
        );
        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Save Assignment Zone'),
        );
    }
    return $form;
}

/**
 * @param $form
 * @param $form_state
 * @return array
 */
function assignment_zone_form_submit($form, &$form_state){
    global $user;
    set_default_time_zone();
    $rowsValues = false;
    if (isset($form_state['values']['rows'])){
        $rowsValues = true;
        $values = $form_state['values']['rows'];
    }else{
        $values = $form_state['values'];
    }
    $assignmentID = $form_state['values']['assignment_id'];
    $month = empty($form_state['values']['month']) ? date('m') : $form_state['values']['month'];
    $year = empty($form_state['values']['year']) ? date('Y') : $form_state['values']['year'];
    if (count($values)){
        if ($rowsValues){
            foreach ($values as $keyIdx => $postData){
                if (count($postData)){
                    foreach ($postData as $zoneDate => $assignmentValue){
                        $explodeZoneDate = explode('-',$zoneDate);
                        $savedData = array();
                        if (isset($explodeZoneDate[1]) && isset($explodeZoneDate[2]) && !isset($explodeZoneDate[3]) && ($assignmentValue === 0 || $assignmentValue > 0)){
                            $savedData['zone'] = $explodeZoneDate[1];
                            $savedData['periode'] = $explodeZoneDate[2];
                            $savedData['bank_in'] = $assignmentValue;
                            $savedData['uid'] = $user->uid;
                            $savedData['assignment_id'] = $assignmentID;
                            $savedData['month'] = $month;
                            $savedData['year'] = $year;
                            $returnData[] = saveTableData('zassigment_zone','admin ikhwan data assignment',$savedData,'id');
                        }else if(isset($explodeZoneDate[1]) && isset($explodeZoneDate[2]) && isset($explodeZoneDate[3]) && ($assignmentValue === 0 || $assignmentValue > 0)){
                            $idAssignmentZone = $explodeZoneDate[3];
                            $savedData['zone'] = $explodeZoneDate[1];
                            $savedData['periode'] = $explodeZoneDate[2];
                            $savedData['bank_in'] = $assignmentValue;
                            $savedData['uid_changed'] = $user->uid;
                            $savedData['assignment_id'] = $assignmentID;
                            $savedData['month'] = $month;
                            $savedData['year'] = $year;
                            $returnData[] = saveTableData('zassigment_zone','admin ikhwan data assignment',$savedData,'id',$idAssignmentZone);
                        }
                    }
                }
            }
        }else{
            foreach ($values as $keyIdx => $assignmentValue){
                if ($assignmentValue > 0){
                    $explodeZoneDate = explode('-',$keyIdx);
                    $savedData = array();
                    if (isset($explodeZoneDate[1]) && isset($explodeZoneDate[2]) && !isset($explodeZoneDate[3])){
                        $savedData['zone'] = $explodeZoneDate[1];
                        $savedData['periode'] = $explodeZoneDate[2];
                        $savedData['bank_in'] = $assignmentValue;
                        $savedData['uid'] = $user->uid;
                        $savedData['assignment_id'] = $assignmentID;
                        $savedData['month'] = $month;
                        $savedData['year'] = $year;
                        $returnData[] = saveTableData('zassigment_zone','admin ikhwan data assignment',$savedData,'id');
                    }else if(isset($explodeZoneDate[1]) && isset($explodeZoneDate[2]) && isset($explodeZoneDate[3])){
                        $idAssignmentZone = $explodeZoneDate[3];
                        $savedData['zone'] = $explodeZoneDate[1];
                        $savedData['periode'] = $explodeZoneDate[2];
                        $savedData['bank_in'] = $assignmentValue;
                        $savedData['uid_changed'] = $user->uid;
                        $savedData['assignment_id'] = $assignmentID;
                        $savedData['month'] = $month;
                        $savedData['year'] = $year;
                        $returnData[] = saveTableData('zassigment_zone','admin ikhwan data assignment',$savedData,'id',$idAssignmentZone);
                    }
                }
            }
        }
    }
    drupal_set_message("Data assignment zone berhasil di simpan");
    return $returnData;
}

/**
 * @param null $assignmentId
 * @param null $zone
 * @param null $month
 * @param null $year
 * @param null $periode
 * @return null
 */
function get_assignment_zone_data($assignmentId = null, $without_month = false, $zone = null, $month = null, $year = null, $periode = null){
    set_default_time_zone();
    $assignmentZone = null;
    if (!empty($assignmentId)){
        if (empty($month)){
            $month = date('n');
        }
        if (empty($year)){
            $year = date('Y');
        }
        $fieldArray = array(
            'id', 'assignment_code', 'zone', 'target', 'bank_in', 'periode', 'created',
            'changed', 'uid', 'uid_changed', 'assignment_id', 'month', 'year'
        );
        $conditionArray = array();
        $consData = new stdClass();
        $consData->fieldName = 'ta.month';
        $consData->value = $month;
        $conditionArray[] = $consData;
        $consData = new stdClass();
        $consData->fieldName = 'ta.year';
        $consData->value = $year;
        $conditionArray[] = $consData;
        $assignmentData = getTableDataByField(
            'zassigment_zone','access ikhwan data assignment',$fieldArray,'zone',
            'assignment_id',$assignmentId,null,$conditionArray
        );
        for ($i = 0;$i < count($assignmentData);$i++){
            if ($without_month){
                $assignmentZone[$assignmentData[$i]->zone][$assignmentData[$i]->periode] = $assignmentData[$i];
            }else {
                $assignmentZone[$assignmentData[$i]->zone][$year][$month][$assignmentData[$i]->periode] = $assignmentData[$i];
            }
        }
    }
    return $assignmentZone;
}

/**
 * @param null $assignmentId
 * @return array
 */
function create_chart_data($assignmentId = null, $zone = null, $month = null, $year = null){
    $chart = null;
    $mktimeYearMonth = mktime(0,0,0,$month,1,$year);
    set_default_time_zone();
    if (!empty($assignmentId)){
        $jenisAssignment = get_assignment_by_id($assignmentId);
        $assignmentData = get_assignment_zone_data($assignmentId, true,$zone, $month,$year);
        $getDataZone = getZoneByCode($zone);
        $namaZone = array();
        if (count($getDataZone)){
            for ($i = 0;$i < count($getDataZone);$i++){
                $namaZone[$getDataZone[$i]->zone] = $getDataZone[$i]->nama;
            }
        }
        if ($jenisAssignment->periode == 0) {
            $lastDate = get_last_day($month, $year);
            $startDate = date('Y-m-',$mktimeYearMonth) . '01';
            $finishDate = date('Y-m-',$mktimeYearMonth) . $lastDate;
            $columnArray = create_date_range_array($startDate, $finishDate, 'UNIX');
        }else if($jenisAssignment->periode == 1){
            $columnArray = array(1,2,3,4,5);
        }else{
            $columnArray = array(1);
        }
        $totalColumn = array();
        if (count($columnArray) > 1){
            $chart = array(
                '#type' => 'chart',
                '#chart_type' => 'column',
                '#chart_library' => 'highcharts', // Allowed values: 'google' or 'highcharts'
                '#title' => '<h3 class="title">'.$jenisAssignment->assignment_name.' '.date('M Y',$mktimeYearMonth).'</h3>',
                '#stacking' => TRUE,
                '#legend' => FALSE,
            );
            if (count($assignmentData)){
                $totalColumn = array();
                foreach ($assignmentData as $keyZone => $assignmentInfo){
                    $assignmentArray = array();
                    for ($i = 0;$i < count($columnArray);$i++){
                        if (isset($assignmentInfo[$columnArray[$i]])){
                            $assignmentArray[] = (float)$assignmentInfo[$columnArray[$i]]->bank_in;
                        }else{
                            $assignmentArray[] = 0;
                        }
                        if (isset($totalColumn[$columnArray[$i]])){
                            if (isset($assignmentInfo[$columnArray[$i]])){
                                $totalColumn[$columnArray[$i]] += (float)$assignmentInfo[$columnArray[$i]]->bank_in;
                            }
                        }else{
                            if (isset($assignmentInfo[$columnArray[$i]])){
                                $totalColumn[$columnArray[$i]] = (float)$assignmentInfo[$columnArray[$i]]->bank_in;
                            }else{
                                $totalColumn[$columnArray[$i]] = 0;
                            }
                        }
                    }
                    $chart[$keyZone] = array(
                        '#type' => 'chart_data',
                        '#title' => $namaZone[$keyZone],
                        '#data' => $assignmentArray,
                        '#prefix' => 'RM.',
                    );
                }
            }
            $chartAxis = array();
            if ($jenisAssignment->periode == 0) {
                for ($i = 0;$i < count($columnArray);$i++){
                    $chartAxis[] = date('d-M', $columnArray[$i]);
                }
            }else if ($jenisAssignment->periode == 1){
                for ($i = 0;$i < count($columnArray);$i++){
                    $chartAxis[] = $jenisAssignment->judul_periode.' '.$columnArray[$i];
                }
            }else{
                $chartAxis[] = date('M Y');
            }
            $chart['xaxis'] = array(
                '#type' => 'chart_xaxis',
                '#labels' => $chartAxis,
            );
        }else{
            $chart = array(
                '#type' => 'chart',
                '#title' => '<h3 class="title">'.$jenisAssignment->assignment_name.' '.date('M Y',$mktimeYearMonth).'</h3>',
                '#chart_type' => 'pie',
                '#chart_library' => 'highcharts', // Allowed values: 'google' or 'highcharts'
                '#legend_position' => 'right',
                '#data_labels' => FALSE,
                '#tooltips' => TRUE,
            );
            $labelsArray = array();
            $valueArray = array();
            $totalColumn = array();
            if (count($assignmentData)){
                foreach ($assignmentData as $keyZone => $assignmentInfo){
                    for ($i = 0;$i < count($columnArray);$i++){
                        if (isset($assignmentInfo[$columnArray[$i]])){
                            $valueArray[] = (float)$assignmentInfo[$columnArray[$i]]->bank_in;
                            $totalColumn[] = (float)$assignmentInfo[$columnArray[$i]]->bank_in;
                        }else{
                            $valueArray[] = 0;
                        }
                    }
                    $labelsArray[] = $namaZone[$keyZone];
                }
            }
            $chart['pie_data'] = array(
                '#type' => 'chart_data',
                '#title' => $jenisAssignment->assignment_name,
                '#labels' => $labelsArray,
                '#data' => $valueArray,
                '#plot_options' => array('pie')
            );
        }
    }
    return array($chart,$totalColumn);
}

/**
 * @param null $assignmentId
 * @return array|null
 */
function total_by_zon_chart($assignmentId = null, $zone = null, $month = null, $year = null){
    $chart = null;
    set_default_time_zone();
    if (!empty($assignmentId)){
        $jenisAssignment = get_assignment_by_id($assignmentId);
        $assignmentData = get_assignment_zone_data($assignmentId, true, $zone, $month, $year);
        $getDataZone = getZoneByCode($zone);
        $namaZone = array();
        if (count($getDataZone)){
            for ($i = 0;$i < count($getDataZone);$i++){
                $namaZone[$getDataZone[$i]->zone] = $getDataZone[$i]->nama;
            }
        }
        if ($jenisAssignment->periode == 0) {
            $chart = array(
                '#type' => 'chart',
                '#chart_type' => 'bar',
                '#chart_library' => 'highcharts', // Allowed values: 'google' or 'highcharts'
                '#title' => '<h3 class="title">'.$jenisAssignment->assignment_name.' '.date('M Y', mktime(
                        0,0,0,$month,1,$year
                    )
                ).'</h3>',
                '#stacking' => TRUE,
                '#legend' => FALSE,
            );
            $totalZone = array();
            $dataZone = array();
            if (count($assignmentData)) {
                $totalZone = array();
                $dataZone = array();
                foreach ($assignmentData as $keyZone => $assignmentInfo) {
                    $dataZone[] = $namaZone[$keyZone];
                    $totalAssigmentZon = 0;
                    if (count($assignmentInfo)){
                        foreach ($assignmentInfo as $assignment){
                            $totalAssigmentZon += (float)$assignment->bank_in;
                        }
                    }
                    $totalZone[] = $totalAssigmentZon;
                }
            }
            $chart[$jenisAssignment->assignment_code] = array(
                '#type' => 'chart_data',
                '#title' => $jenisAssignment->assignment_name,
                '#data' => $totalZone,
                '#prefix' => 'RM.',
            );
            $chart['xaxis'] = array(
                '#type' => 'chart_xaxis',
                '#labels' => $dataZone,
            );
        }
    }
    return $chart;
}

function menu_dashboard_report(){
    $dashboardMenu = array();
    $assignmentButton = array(
        'element' => array(
            '#attributes' => array(
                'class' => array('btn-primary'),
                'id' => 'assigment-chart',
                'name' => 'assigment-chart',
            ),
            '#button_type' => 'button',
            '#value' => t('ASSIGNMENT ZON'),
        )
    );
    $dashboardMenu[] = theme('button', $assignmentButton);
    $overheadButton = array(
        'element' => array(
            '#attributes' => array(
                'class' => array('btn-success'),
                'id' => 'overhead-chart',
                'name' => 'overhead-chart',
            ),
            '#button_type' => 'button',
            '#value' => t('OVERHEAD ZON'),
        )
    );
    $dashboardMenu[] = theme('button', $overheadButton);
    $cashflowButton = array(
        'element' => array(
            '#attributes' => array(
                'class' => array('btn-warning'),
                'id' => 'cashflow-chart',
                'name' => 'cashflow-chart',
            ),
            '#button_type' => 'button',
            '#value' => t('CASHFLOW ZON'),
        )
    );
    $dashboardMenu[] = theme('button', $cashflowButton);
    return $dashboardMenu;
}

/**
 * @return string
 */
function ikhwan_chart_dashboard($month = null, $year = null, $zone = null){
    if (empty($month) && empty($year)){
        if (date('j') <= 25){
            $month = date('n', mktime(0,0,0,date('n') - 1,1,date('Y')));
            $year = date('Y', mktime(0,0,0,date('n') - 1,1,date('Y')));
        }else{
            $month = date('n');
            $year = date('Y');
        }
    }
    drupal_add_js(
        array(
            'month' => $month,
            'year' => $year,
        )
        , 'setting'
    );
    $module_path = drupal_get_path('module','ikhwan_core');
    $csspath = $module_path.'/css/custom-style.css';
    drupal_add_css($csspath);
    $jspath = $module_path.'/js/dashboard_menu.js';
    drupal_add_js($jspath,array('scope' => 'footer', 'weight' => 5));
    $jspath = $module_path.'/js/dashboard.js';
    drupal_add_js($jspath,array('scope' => 'footer', 'weight' => 5));
    $jenisAssignment = get_assignment_by_id();
    drupal_set_title('');
    $monthBeforeButtonVar = array(
        'element' => array(
            '#attributes' => array(
                'class' => array('btn-warning btn-sm'),
                'id' => 'month-before',
                'name' => 'month-before',
            ),
            '#button_type' => 'button',
            '#value' => date('F', mktime(0,0,0,date('n') -1,1,date('Y'))),
        )
    );
    $monthBeforeButton = theme('button', $monthBeforeButtonVar);
    $monthBefore = date('Y', mktime(0,0,0,date('n') - 1, 1, date('Y'))).'_'.date('n', mktime(0,0,0,date('n') - 1, 1, date('Y')));
    $monthBeforeButton .= '<input type="hidden" id="month_before" name="month_before" value="'.$monthBefore.'">';
    $currMonthButtonVar = array(
        'element' => array(
            '#attributes' => array(
                'class' => array('btn-primary btn-sm'),
                'id' => 'curr-month',
                'name' => 'curr-month',
            ),
            '#button_type' => 'button',
            '#value' => date('F'),
        )
    );
    $currMonthButton = theme('button', $currMonthButtonVar);
    $currMonthButton .= '<input type="hidden" id="curr_month" name="curr_month" value="'.date('Y').'_'.date('n').'">';
    $dashBoard = '<div class="">';
    $dashBoard .= '<div class="col-md-12 main-chart">';
    $dashBoard .= assignment_all_3d_column_chart($month, $year, $zone);
    $dashBoard .= '</div>';
    $arrayColor = array('#434348','#90ed7d','#7cb5ec');
    for ($i = 0;$i < count($jenisAssignment);$i++){
        if (!isset($arrayColor[$i])){
            $arrayColor[$i] = '#7cb5ec';
        }
        $dashBoard .= '<div class="col-md-4">';
        $dashBoard .= assignment_total_3d_column_chart($jenisAssignment[$i]->id, $month, $year, $zone, $arrayColor[$i]);
        $dashBoard .= '</div>';
    }
    $dashBoard .= '</div>';
    $chartMenu = implode('&nbsp;',menu_dashboard_report());
    return '<div>'.$chartMenu.'</div>'.$monthBeforeButton.$currMonthButton.$dashBoard;
}
function pengeluaran_assignment($assigmentId = null){
    if (!empty($assigmentId)){
        drupal_add_js(
            array(
                'assigmentId' => $assigmentId,
            )
            , 'setting');
    }
    $module_path = drupal_get_path('module','ikhwan_core');
    $csspath = $module_path.'/css/custom-style.css';
    drupal_add_css($csspath);
    $jspath = $module_path.'/js/pengeluaran_assignment.js';
    drupal_add_js($jspath,array('scope' => 'footer', 'weight' => 5));
    $jenisAssignment = get_assignment_by_id();
    $assignmentOut = '<div class="col-md-12">';
    for ($i = 0;$i < count($jenisAssignment);$i++){
        $assignmentButtonVar = array(
            'element' => array(
                '#attributes' => array(
                    'class' => array('btn-primary btn-sm input-assignment-out'),
                    'id' => 'assignmentout-'.$jenisAssignment[$i]->id,
                    'name' => 'assignmentout-'.$jenisAssignment[$i]->id,
                ),
                '#button_type' => 'button',
                '#value' => $jenisAssignment[$i]->assignment_name,
            )
        );
        $newAssignmentButton = theme('button', $assignmentButtonVar);
        $assignmentOut .= $newAssignmentButton;
    }
    $assignmentOut .= '</div>';
    $assignmentOut .= '<div class="col-md-12">';
    if (!empty($assigmentId)){
        $variables['id'] = $assigmentId;
        $jenisAssignment = get_assignment_by_id($variables['id']);
        drupal_add_js(
            array(
                'jenisAssignment' => $jenisAssignment,
            )
            , 'setting');
        $form = drupal_get_form('form_pengeluaran_assignment',$variables);
        $formAssignmentout = drupal_render($form);
        $assignmentOut .= $formAssignmentout;
    }else{
        $variables['content'] = t('Klik tombol diatas untuk melakukan input pengeluaran assignment');
        $form = drupal_get_form('infoForm',$variables);
        $formAssignment = drupal_render($form);
        $assignmentOut .= $formAssignment;
    }
    $assignmentOut .= '</div>';
    return $assignmentOut;
}
function form_pengeluaran_assignment($form, &$form_state, $variables = null){
    set_default_time_zone();
    if (isset($variables['id']) && !empty($variables['id'])){
        $jenisAssignment = get_assignment_by_id($variables['id']);
        if (isset($variables['offset'])){
            $offset = $variables['offset'];
        }else{
            $offset = 0;
        }
        $tableHeader = create_pengeluaran_assignment_header();
        $assignmentOut = get_assignment_out_data($variables['id']);
        $tableRow = create_assignment_out_row($assignmentOut,$variables['id']);
        $tableFooter = create_pengeluaran_assignment_footer();
        $form['pengeluaran'] = array(
            '#type' => 'fieldset',
            '#title' => 'Pengeluaran '.$jenisAssignment->assignment_name.' '.date('M').' '.date('Y'),
            '#collapsible' => FALSE, // Added
            '#collapsed' => FALSE,  // Added
        );
        $form['pengeluaran']['submit2'] = array(
            '#type' => 'submit',
            '#value' => t('Save Pengeluaran '.$jenisAssignment->assignment_code),
        );
        $form['pengeluaran']['assignment_id'] = array(
            '#type' => 'hidden',
            '#default_value' => $variables['id'],
        );
        $form['pengeluaran']['table'] = array(
            '#theme' => 'formtable_form_table',
            '#header' => $tableHeader,
            'rows' => $tableRow,
            '#footer' => $tableFooter,
        );
        $form['pengeluaran']['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Save Pengeluaran '.$jenisAssignment->assignment_code),
        );
    }
    return $form;
}

/**
 * @return array
 */
function create_pengeluaran_assignment_header(){
    $tableHeader = array();
    $tableHeader[] = array(
        'data' => t('H/B.'),
        'class' => array(
            'col-number'
        ),
    );
    $tableHeader[] = array(
        'data' => t('BIL.'),
        'class' => array(
            'col-number'
        ),
    );
    $tableHeader[] = t('PERKARA');
    $tableHeader[] = array(
        'data' => t('NILAIAN'),
        'class' => array(
            'col-input-number'
        ),
    );
    $tableHeader[] = array(
        'data' => t('TOTAL HARIAN'),
        'class' => array(
            'col-total-number'
        ),
    );
    return $tableHeader;
}

/**
 * @return array
 */
function create_pengeluaran_assignment_footer(){
    $tableFooter = array();
    $tableFooter[] = '&nbsp;';
    $tableFooter[] = '&nbsp;';
    $tableFooter[] = t('JUMLAH PERBELANJAAN KESELURUHAN');
    $tableFooter[] = 'RM. ';
    $tableFooter[] = '<div id="grand-total" class="align-right bolder">0</div>';
    return array($tableFooter);
}
function get_assignment_out_data($assignmentId = null){
    $assigmentOutData = null;
    if (!empty($assignmentId)){
        $tableName = 'zpengeluaran_assignment';
        $fieldArray = array(
            'id', 'assignment_id', 'no', 'perkara', 'jumlah', 'tglpengeluaran',
            'uid', 'uid_changed', 'created', 'changed'
        );
        $conditionArray = array();
        $consData = new stdClass();
        $consData->fieldName = 'ta.assignment_id';
        $consData->value = $assignmentId;
        $conditionArray[] = $consData;
        $fieldOrder = array(
            array ('fieldname' => 'tglpengeluaran'),
            array('fieldname' => 'no'),
        );
        $assignmentRecord = getTableDataByField(
            $tableName,'access ikhwan data assignment',
            $fieldArray,$fieldOrder,null,null,null,
            $conditionArray
        );
        if (count($assignmentRecord)){
            for ($i = 0;$i < count($assignmentRecord);$i++){
                $assigmentOutData[$assignmentRecord[$i]->tglpengeluaran][$assignmentRecord[$i]->no] = $assignmentRecord[$i];
            }
        }
    }
    return $assigmentOutData;
}
function create_assignment_out_row($assignmentData = null, $assignmentId = null){
    set_default_time_zone();
    $tableRow = array();
    $lastDate = get_last_day(date('n'),date('Y'));
    $startDate = date('Y-m-').'01';
    $finishDate = date('Y-m-').$lastDate;
    $dateArray = create_date_range_array($startDate,$finishDate, 'j');
    $dateArray2 = create_date_range_array($startDate,$finishDate, 'UNIX');
    $tableRow['#tree'] = TRUE;
    $counter = 0;
    for ($i = 0;$i < count($dateArray);$i++){
        if (isset($assignmentData[$dateArray2[$i]])){
            $assignmentOutInputed = $assignmentData[$dateArray2[$i]];
            for ($j = 0;$j < count($assignmentOutInputed);$j++){
                $rowData = array();
                $bilangan = $j + 1;
                if ($j == 0){
                    $rowData['c-'.$assignmentId.'-'.$dateArray2[$i]] = array(
                        '#type' => 'item', '#title' => $dateArray[$i].'hb',
                        '#attributes' => array('outerclass' => array('align-right')),
                    );
                }else{
                    $rowData['c-'.$assignmentId.'-'.$dateArray2[$i]] = array(
                        '#type' => 'item', '#title' => '&nbsp;',
                    );
                }
                $rowData['no-'.$assignmentId.'-'.$dateArray2[$i].'-'.($j + 1).'-'.$assignmentOutInputed[$bilangan]->id] = array(
                    '#type' => 'textfield',
                    '#default_value' => $assignmentOutInputed[$bilangan]->no,
                    '#attributes' => array(
                        'class' => array(
                            'align-right',
                        ),
                        'readonly' => 'readonly',
                    ),
                );
                $rowData['perkara-'.$assignmentId.'-'.$dateArray2[$i].'-'.($j + 1)] = array(
                    '#type' => 'textfield',
                    '#default_value' => $assignmentOutInputed[$bilangan]->perkara,
                );
                $rowData['jumlah-'.$assignmentId.'-'.$dateArray2[$i].'-'.($j + 1)] = array(
                    '#type' => 'textfield',
                    '#default_value' => $assignmentOutInputed[$bilangan]->jumlah,
                    '#attributes' => array(
                        'class' => array(
                            'align-right','jumlah-'.$dateArray2[$i],'jumlah-pengeluaran',
                        ),
                    ),
                );
                $rowData['total-'.$assignmentId.'-'.$dateArray2[$i].'-'.($j + 1)] = array(
                    '#type' => 'item',
                    '#title' => '',
                );
                $tableRow['r'.($counter)] = $rowData;
                $counter++;
            }
            $counter++;
            for ($j = 0;$j < 2;$j++){
                $rowData = array();
                $rowData['c-'.$assignmentId.'-'.$dateArray2[$i]] = array(
                    '#type' => 'item', '#title' => '&nbsp;',
                );
                $rowData['no-'.$assignmentId.'-'.$dateArray2[$i].'-'.$counter] = array(
                    '#type' => 'textfield',
                    '#default_value' => $counter,
                    '#attributes' => array(
                        'class' => array(
                            'align-right'
                        ),
                    ),
                );
                $rowData['perkara-'.$assignmentId.'-'.$dateArray2[$i].'-'.$counter] = array(
                    '#type' => 'textfield',
                    '#default_value' => '',
                );
                $rowData['jumlah-'.$assignmentId.'-'.$dateArray2[$i].'-'.$counter] = array(
                    '#type' => 'textfield',
                    '#default_value' => 0,
                    '#attributes' => array(
                        'class' => array(
                            'align-right','jumlah-'.$dateArray2[$i],'jumlah-pengeluaran',
                        ),
                    ),
                );
                if ($j == 1) {
                    $rowData['total-' . $assignmentId . '-' . $dateArray2[$i] . '-' . $counter] = array(
                        '#type' => 'textfield',
                        '#default_value' => 0,
                        '#attributes' => array(
                            'class' => array(
                                'align-right',
                                'total-'.$dateArray2[$i],
                                'total-harian',
                            ),
                            'readonly' => 'readonly',
                        ),
                    );
                    $tableRow['r' . ($counter)] = $rowData;
                }else {
                    $rowData['total-' . $assignmentId . '-' . $dateArray2[$i] . '-' . $counter] = array(
                        '#type' => 'item',
                        '#title' => '',
                    );
                }
                $tableRow['r'.($counter)] = $rowData;
                $counter++;
            }
        }else{
            for ($j = 0;$j < 5;$j++){
                $rowData = array();
                if ($j == 0){
                    $rowData['c-'.$assignmentId.'-'.$dateArray2[$i]] = array(
                        '#type' => 'item', '#title' => $dateArray[$i].'hb',
                        '#attributes' => array('outerclass' => array('align-right')),
                    );
                }else{
                    $rowData['c-'.$assignmentId.'-'.$dateArray2[$i]] = array(
                        '#type' => 'item', '#title' => '&nbsp;',
                    );
                }
                $rowData['no-'.$assignmentId.'-'.$dateArray2[$i].'-'.($j + 1)] = array(
                    '#type' => 'textfield',
                    '#default_value' => ($j + 1),
                    '#attributes' => array(
                        'class' => array(
                            'align-right',
                        ),
                        'readonly' => 'readonly',
                    ),
                );
                $rowData['perkara-'.$assignmentId.'-'.$dateArray2[$i].'-'.($j + 1)] = array(
                    '#type' => 'textfield',
                    '#default_value' => '',
                );
                $rowData['jumlah-'.$assignmentId.'-'.$dateArray2[$i].'-'.($j + 1)] = array(
                    '#type' => 'textfield',
                    '#default_value' => 0,
                    '#attributes' => array(
                        'class' => array(
                            'align-right','jumlah-pengeluaran','jumlah-'.$dateArray2[$i],
                        ),
                    ),
                );
                if ($j == 4) {
                    $rowData['total-' . $assignmentId . '-' . $dateArray2[$i]] = array(
                        '#type' => 'textfield',
                        '#default_value' => 0,
                        '#attributes' => array(
                            'class' => array(
                                'align-right',
                                'total-'.$dateArray2[$i],
                                'total-harian',
                            ),
                            'readonly' => 'readonly',
                        ),
                    );
                }else{
                    $rowData['total-' . $assignmentId . '-' . $dateArray2[$i] . '-' . ($j + 1)] = array(
                        '#type' => 'item',
                        '#title' => '',
                    );
                }
                $tableRow['r'.($counter)] = $rowData;
                $counter++;
            }
        }
    }
    return $tableRow;
}
function form_pengeluaran_assignment_submit($form, &$form_state){
    global $user;
    set_default_time_zone();
    if (isset($form_state['values']['rows'])) {
        $values = $form_state['values']['rows'];
    } else {
        $values = $form_state['values'];
    }
    $returnData = array();
    if (count($values)){
        foreach($values as $idx => $rowData){
            if (count($rowData)){
                $savedData = array();
                $updateData = false;
                foreach ($rowData as $idxRow => $dataValues){
                    $splitData = explode('-',$idxRow);
                    if (count($splitData) == 5){
                        $updateData = true;
                        $idAssignmentOut = $splitData[4];
                    }
                    $savedData[trim($splitData[0])] = $dataValues;
                    $savedData['assignment_id'] = $splitData[1];
                    $savedData['tglpengeluaran'] = $splitData[2];
                }
                if ($savedData['jumlah'] > 0 && $updateData){
                    $returnData[] = saveTableData(
                        'zpengeluaran_assignment','admin ikhwan data assignment',
                        $savedData,'id',$idAssignmentOut
                    );
                }else if($savedData['jumlah'] > 0 && !$updateData){
                    $returnData[] = saveTableData(
                        'zpengeluaran_assignment','admin ikhwan data assignment',
                        $savedData,'id'
                    );
                }
            }
        }
    }
    return $returnData;
}
function create_array_zone(){
    $dataZone = getZoneByCode();
    $arrayZone = array();
    for ($i = 0;$i < count($dataZone);$i++){
        $arrayZone[$dataZone[$i]->zone] = $dataZone[$i]->nama;
    }
    return $arrayZone;
}
function get_data_zone($arrayZone = null){
    $arrayfield = array('id', 'zone', 'nama');
    $dataZone = getZoneByCode();
    if (!empty($arrayZone)){
        $newDataZone = array();
        for ($i = 0;$i < count($dataZone);$i++){
            if (in_array($dataZone[$i]->zon, $arrayZone)){
                $newDataZone[] = $dataZone[$i];
            }
        }
        $dataZone = $newDataZone;
    }
    return $dataZone;
}
function cek_keuangan_zone(){
    global $user;
    $keuanganZone = false;
    if (in_array('keuangan-zone',$user->roles)){
        $keuanganZone = true;
    }
    return $keuanganZone;
}
function get_user_zone(){
    global $user;
    $userZonePremis  = get_user_zone_premis();
    $userZone = false;
    if (count($userZonePremis)){
        $userZone = true;
    }
    return array($userZone, $userZonePremis);
}
function create_user_zone_header(){
    $tableHeader = array();
    $tableHeader[] = array(
        'data' => t('NO.'),
        'class' => array(
            'col-number',
        ),
    );
    $tableHeader[] = array(
        'data' => t('NAMA'),
        'class' => array(
            'col-title',
        ),
    );
    $tableHeader[] = array(
        'data' => t('USER ID'),
        'class' => array(
            'col-input-number',
        ),
    );
    $tableHeader[] = t('ZONE');
    $tableHeader[] = array(
        'data' => t('PREMIS'),
    );
    return $tableHeader;
}
function create_user_zone_footer(){
    $tableHeader = array();
    $tableHeader[] = array(
        'data' => t('NO.'),
        'class' => array(
            'col-number',
        ),
    );
    $tableHeader[] = array(
        'data' => t('NAMA'),
        'class' => array(
            'col-title',
        ),
    );
    $tableHeader[] = array(
        'data' => t('USER ID'),
        'class' => array(
            'col-input-number',
        ),
    );
    $tableHeader[] = t('ZONE');
    $tableHeader[] = array(
        'data' => t('PREMIS'),
    );
    return $tableHeader;
}
function get_user_zone_premis(){
    global $user;
    $userZone = array();
    $tableName = 'zuser_zone';
    $fieldArray = array(
        'id', 'nama', 'related_uid', 'zone', 'id_premis',
        'uid', 'uid_changed', 'created', 'changed'
    );
    $userZonePremis = getTableDataByField(
        $tableName, 'access ikhwan user zone',
        $fieldArray, 'nama', 'related_uid', $user->uid
    );
    if (count($userZonePremis)){
        $userZone['zone'] = explode(',',$userZonePremis[0]->zone);
        $userZone['id_premis'] = explode(',',$userZonePremis[0]->id_premis);
    }
    return $userZone;
}
function get_user_zone_list(){
    $userZonePremis = array();
    $tableName = 'zuser_zone';
    $fieldArray = array(
        'id', 'nama', 'related_uid', 'zone', 'id_premis',
        'uid', 'uid_changed', 'created', 'changed'
    );
    $userZonePremis = getTableDataByField(
        $tableName,'access ikhwan user zone',
        $fieldArray,'nama'
    );
    return $userZonePremis;
}
function create_user_zone_row(){
    $tableRow['#tree'] = TRUE;
    $userList = get_user_zone_list();
    $fieldArray = array('uid','name');
    $userArray = getTableDataByField('users','access ikhwan user zone',$fieldArray,'name');
    $userSelection = array();
    for ($i = 0;$i < count($userArray);$i++){
        $userSelection[$userArray[$i]->uid] = $userArray[$i]->name;
    }
    $premisArr = get_data_premis_by_id();
    $premisOpt = array();
    for ($i = 0;$i < count($premisArr);$i++){
        $premisOpt[$premisArr[$i]->id] = $premisArr[$i]->nama;
    }
    $zoneArr = getZoneByCode();
    $zoneOpt = array();
    for ($i = 0;$i < count($zoneArr);$i++){
        $zoneOpt[$zoneArr[$i]->zone] = $zoneArr[$i]->nama;
    }
    $counter = 1;
    for ($i = 0;$i < 1;$i++){
        $colData = array();
        $zoneUser = null;
        $premisUser = null;
        $colData['no-new-'.$counter] = array(
            '#type' => 'item', '#title' => $counter,
            '#attributes' => array('outerclass' => array('align-right')),
        );
        $colData['nama-new-'.$counter] = array(
            '#type' => 'textfield',
            '#default_value' => '',
        );
        $colData['related_uid-new-'.$counter] = array(
            '#type' => 'select',
            '#default_value' => null,
            '#options' => $userSelection,
            '#select2' => array(
                'placeholder' => 'Pilih User Terkait',
                'width' => 300,
            ),
        );
        $colData['zone-new-'.$counter] = array(
            '#type' => 'select',
            '#default_value' => $zoneUser,
            '#options' => $zoneOpt,
            '#multiple' => TRUE,
            '#select2' => array(
                'placeholder' => 'Pilih Zone Terkait',
                'width' => 300,
            ),
            '#attributes' => array('class' => array('select-zone',),),
        );
        $colData['premis-new-'.$counter] = array(
            '#type' => 'select',
            '#default_value' => $premisUser,
            '#options' => $premisOpt,
            '#multiple' => TRUE,
            '#select2' => array(
                'placeholder' => 'Pilih Premis Terkait',
                'width' => 300,
            ),
            '#attributes' => array('class' => array('select-premis',),),
        );
        $tableRow['r-'.$counter] = $colData;
        $counter++;
    }
    for ($i = 0;$i < count($userList);$i++){
        $colData = array();
        $zoneUser = explode(',',$userList[$i]->zone);
        $premisUser = explode(',',$userList[$i]->id_premis);
        $colData['no-'.$userList[$i]->id.'-'.$counter] = array(
            '#type' => 'item', '#title' => $counter,
            '#attributes' => array('outerclass' => array('align-right')),
        );
        $colData['nama-'.$userList[$i]->id.'-'.$counter] = array(
            '#type' => 'textfield',
            '#default_value' => $userList[$i]->nama,
        );
        $colData['related_uid-'.$userList[$i]->id.'-'.$counter] = array(
            '#type' => 'select',
            '#default_value' => $userList[$i]->related_uid,
            '#options' => $userSelection,
            '#select2' => array(
                'placeholder' => 'Pilih User Terkait',
                'width' => 300,
            ),
        );
        $colData['zone-'.$userList[$i]->id.'-'.$counter] = array(
            '#type' => 'select',
            '#default_value' => $zoneUser,
            '#options' => $zoneOpt,
            '#multiple' => TRUE,
            '#select2' => array(
                'placeholder' => 'Pilih Zone Terkait',
                'width' => 300,
            ),
            '#attributes' => array('class' => array('select-zone',),),
        );
        $colData['premis-'.$userList[$i]->id.'-'.$counter] = array(
            '#type' => 'select',
            '#default_value' => $premisUser,
            '#options' => $premisOpt,
            '#multiple' => TRUE,
            '#select2' => array(
                'placeholder' => 'Pilih Premis Terkait',
                'width' => 300,
            ),
            '#attributes' => array('class' => array('select-premis',),),
        );
        $tableRow['r-'.$counter] = $colData;
        $counter++;
    }
    return $tableRow;
}
function form_user_zone($form, &$form_state, $variables = null){
    $tableHeader = create_user_zone_header();
    $tableRow = create_user_zone_row();
    $tableFooter[] = create_user_zone_footer();
    $form['basic'] = array(
        '#type' => 'fieldset',
        '#title' => 'DATA USER ZONE/PREMIS',
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
    );
    $form['basic']['table'] = array(
        '#theme' => 'formtable_form_table',
        '#header' => $tableHeader,
        'rows' => $tableRow,
        '#footer' => $tableFooter,
    );
    $form['basic']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('SAVE USER ZONE'),
    );
    return $form;
}
function form_user_zone_submit($form, &$form_state)
{
    global $user;
    if (isset($form_state['values']['rows'])) {
        $values = $form_state['values']['rows'];
    } else {
        $values = $form_state['values'];
    }
    $returnData = array();
    $fieldArray = array('id');
    if (count($values)) {
        foreach ($values as $idx => $userValue){
            if (count($userValue)){
                $savedData = array();
                foreach ($userValue as $idxValue => $fieldValue){
                    $explodeKey = explode('-', $idxValue);
                    if ($explodeKey[0] != 'zone' && $explodeKey[0] != 'premis'){
                        if ($explodeKey[1] == 'new'){
                            $savedData[$explodeKey[0]] = $fieldValue;
                        }else{
                            $savedData[$explodeKey[0]] = $fieldValue;
                            $savedData['id'] = $explodeKey[1];
                        }
                    }else{
                        if ($explodeKey[1] == 'new'){
                            if ($explodeKey[0] == 'premis'){
                                if (count($fieldValue)){
                                    $premisCommaSeparated = '';
                                    $counterPremis = 0;
                                    foreach ($fieldValue as $premisId){
                                        if ($counterPremis == 0) {
                                            $premisCommaSeparated = $premisId;
                                        }else{
                                            $premisCommaSeparated .= ','.$premisId;
                                        }
                                        $counterPremis++;
                                    }
                                }else{
                                    $premisCommaSeparated = empty($fieldValue) ? null : $fieldValue;
                                }
                                $savedData['id_premis'] = $premisCommaSeparated;
                            }else{
                                $savedData[$explodeKey[0]] = implode(',', $fieldValue);
                            }
                        }else{
                            if ($explodeKey[0] == 'premis'){
                                if (count($fieldValue)){
                                    $premisCommaSeparated = '';
                                    $counterPremis = 0;
                                    foreach ($fieldValue as $premisId){
                                        if ($counterPremis == 0) {
                                            $premisCommaSeparated = $premisId;
                                        }else{
                                            $premisCommaSeparated .= ','.$premisId;
                                        }
                                        $counterPremis++;
                                    }
                                }else{
                                    $premisCommaSeparated = empty($fieldValue) ? null : $fieldValue;
                                }
                                $savedData['id_premis'] = $premisCommaSeparated;
                            }else{
                                $savedData[$explodeKey[0]] = implode(',', $fieldValue);
                            }
                            $savedData['id'] = $explodeKey[1];
                        }
                    }
                }
                if (isset($savedData['id']) && !empty($savedData['id'])){
                    if (isset($savedData['nama']) && !empty($savedData['nama'])){
                        $returnData[] = saveTableData('zuser_zone', 'admin ikhwan user zone', $savedData,'id',$savedData['id']);
                    }else{
                        $conditionData = array();
                        $consObj = new stdClass();
                        $consObj->fieldName = 'id';
                        $consObj->fieldValue = $savedData['id'];
                        $consObj->status = 'and';
                        $conditionData[] = $consObj;
                        $returnData[] = deleteTableData('zuser_zone', 'admin ikhwan user zone', $conditionData);
                        drupal_set_message('Data user berhasil dihapus...!!','error');
                    }
                }else{
                    $userZonePremis = getTableDataByField(
                        'zuser_zone', 'access ikhwan user zone',
                        $fieldArray, 'nama', 'related_uid', $savedData['related_uid']
                    );
                    if (isset($savedData['nama']) && !empty($savedData['nama'])) {
                        if (!count($userZonePremis)){
                            $returnData[] = saveTableData('zuser_zone', 'admin ikhwan user zone', $savedData, 'id');
                        }else{
                            drupal_set_message('User sudah pernah digunakan, mohon pilih user lain nya...!!!','error');
                        }

                    }
                }
            }
        }
        drupal_set_message('Data user berhasil disimpan/update...');
    }
    return $returnData;
}
function user_zone(){
    $core_module_path = drupal_get_path('module','ikhwan_core');
    $csspath = $core_module_path.'/css/custom-style.css';
    drupal_add_css($csspath);
    $jspath = $core_module_path.'/js/list_user_zone.js';
    drupal_add_js($jspath,array('scope' => 'footer', 'weight' => 5));
    $formUserZone = '';
    $form = drupal_get_form('form_user_zone');
    $formUserZone .= drupal_render($form);
    return $formUserZone;
}
function overhead_dashboard($month = null, $year = null, $zone = null){
    if (empty($month) && empty($year)){
        if (date('j') <= 25){
            $month = date('n', mktime(0,0,0,date('n') - 1,1,date('Y')));
            $year = date('Y', mktime(0,0,0,date('n') - 1,1,date('Y')));
        }else{
            $month = date('n');
            $year = date('Y');
        }
    }
    drupal_add_js(
        array(
            'month' => $month,
            'year' => $year,
        )
        , 'setting'
    );
    $module_path = drupal_get_path('module','ikhwan_core');
    $csspath = $module_path.'/css/custom-style.css';
    drupal_add_css($csspath);
    $jspath = $module_path.'/js/dashboard_menu.js';
    drupal_add_js($jspath,array('scope' => 'footer', 'weight' => 5));
    $jspath = $module_path.'/js/dashboard_overhead.js';
    drupal_add_js($jspath,array('scope' => 'footer', 'weight' => 5));
    $monthBeforeButtonVar = array(
        'element' => array(
            '#attributes' => array(
                'class' => array('btn-warning btn-sm'),
                'id' => 'month-before',
                'name' => 'month-before',
            ),
            '#button_type' => 'button',
            '#value' => date('F', mktime(0,0,0,date('n') -1,1,date('Y'))),
        )
    );
    $monthBeforeButton = theme('button', $monthBeforeButtonVar);
    $monthBefore = date('Y', mktime(0,0,0,date('n') - 1, 1, date('Y'))).'_'.date('n', mktime(0,0,0,date('n') - 1, 1, date('Y')));
    $monthBeforeButton .= '<input type="hidden" id="month_before" name="month_before" value="'.$monthBefore.'">';
    $currMonthButtonVar = array(
        'element' => array(
            '#attributes' => array(
                'class' => array('btn-primary btn-sm'),
                'id' => 'curr-month',
                'name' => 'curr-month',
            ),
            '#button_type' => 'button',
            '#value' => date('F'),
        )
    );
    $currMonthButton = theme('button', $currMonthButtonVar);
    $currMonthButton .= '<input type="hidden" id="curr_month" name="curr_month" value="'.date('Y').'_'.date('n').'">';
    $dashBoard = '<div class="col-md-12 main-chart">';
    $dashBoard .= overhead_total_3d_column_chart($month, $year, $zone,'#7cb5ec');
    $dashBoard .= '</div>';
    $chartMenu = implode('&nbsp;',menu_dashboard_report());
    return '<div>'.$chartMenu.'</div>'.$monthBeforeButton.$currMonthButton.$dashBoard;
}
function set_user_number_format(&$desIn, &$desOut, &$separator){
    global $user;
    $lang = $user->language;
    if ($lang == 'id' || empty($lang)){
        $desIn = '.';
        $desOut = ',';
        $separator = '.';
    }else{
        $desIn = '.';
        $desOut = '.';
        $separator = ',';
    }
    drupal_add_js(
        array(
            'desIn' => $desIn,
            'desOut' => $desOut,
            'separator' => $separator,
            'language' => $lang,
        )
        , 'setting'
    );
}
function cashflow_dashboard($month = null, $year = null, $zone = null){
    if (empty($month) && empty($year)){
        if (date('j') <= 25){
            $month = date('n', mktime(0,0,0,date('n') - 1,1,date('Y')));
            $year = date('Y', mktime(0,0,0,date('n') - 1,1,date('Y')));
        }else{
            $month = date('n');
            $year = date('Y');
        }
    }
    drupal_add_js(
        array(
            'month' => $month,
            'year' => $year,
        )
        , 'setting'
    );
    $module_path = drupal_get_path('module','ikhwan_core');
    $csspath = $module_path.'/css/custom-style.css';
    drupal_add_css($csspath);
    $jspath = $module_path.'/js/dashboard_menu.js';
    drupal_add_js($jspath,array('scope' => 'footer', 'weight' => 5));
    $jspath = $module_path.'/js/dashboard_cashflow.js';
    drupal_add_js($jspath,array('scope' => 'footer', 'weight' => 5));
    $monthBeforeButtonVar = array(
        'element' => array(
            '#attributes' => array(
                'class' => array('btn-warning btn-sm'),
                'id' => 'month-before',
                'name' => 'month-before',
            ),
            '#button_type' => 'button',
            '#value' => date('F', mktime(0,0,0,date('n') -1,1,date('Y'))),
        )
    );
    $monthBeforeButton = theme('button', $monthBeforeButtonVar);
    $monthBefore = date('Y', mktime(0,0,0,date('n') - 1, 1, date('Y'))).'_'.date('n', mktime(0,0,0,date('n') - 1, 1, date('Y')));
    $monthBeforeButton .= '<input type="hidden" id="month_before" name="month_before" value="'.$monthBefore.'">';
    $currMonthButtonVar = array(
        'element' => array(
            '#attributes' => array(
                'class' => array('btn-primary btn-sm'),
                'id' => 'curr-month',
                'name' => 'curr-month',
            ),
            '#button_type' => 'button',
            '#value' => date('F'),
        )
    );
    $currMonthButton = theme('button', $currMonthButtonVar);
    $currMonthButton .= '<input type="hidden" id="curr_month" name="curr_month" value="'.date('Y').'_'.date('n').'">';
    $dashBoard = '<div class="col-md-12 main-chart">';
    $dashBoard .= cash_flow_zon_chart($month, $year, $zone);
    $dashBoard .= '</div>';
    $chartMenu = implode('&nbsp;',menu_dashboard_report());
    return '<div>'.$chartMenu.'</div>'.$monthBeforeButton.$currMonthButton.$dashBoard;
}