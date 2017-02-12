<?php

function setoran_pengeluaran($idPremis = null, $month = null, $year = null){
    $module_path = drupal_get_path('module','ikhwan_core');
    $csspath = $module_path.'/css/custom-style.css';
    drupal_add_css($csspath);
    $jspath = $module_path.'/js/jquery.masknumber.js';
    drupal_add_js($jspath,array('scope' => 'footer', 'weight' => 5));
    $jspath = $module_path.'/js/notify.js';
    drupal_add_js($jspath,array('scope' => 'footer', 'weight' => 5));
    $module_path  = drupal_get_path('module','setoran_pengeluaran');
    $jspath = $module_path.'/js/input_setoran.js';
    drupal_add_js($jspath,array('scope' => 'footer', 'weight' => 5));
    $variables['id_premis'] = $idPremis;
    $variables['month'] = $month;
    $variables['year'] = $year;
    $form = drupal_get_form('setoran_form',$variables);
    $formAssignment = '<div class="col-md-5">'.drupal_render($form).'</div>';
    return $formAssignment;
}

function setoran_form_header(){
    $tableHeader = array();
    $tableHeader[] = array(
        'data' => t('TANGGAL'),
            'class' => array(
                'col-input-number-small',
            ),
        );
        $tableHeader[] = t('KETERANGAN');
        $tableHeader[] = array(
            'data' => t('NILAI'),
            'class' => array(
                'col-input-number',
            ),
        );
        $tableHeader[] = array(
            'data' => t('TOTAL'),
            'class' => array(
                'col-input-number',
            ),
        );
    return $tableHeader;
}
function setoran_form_footer(){
    $tableHeader = array();
    $tableHeader[] = array(
        'data' => t('TOTAL SETORAN'),
        'colspan' => 2,
    );
    $tableHeader[] = array(
        'data' => '<div id="grand-total" class="align-right"></div>',
        'class' => array(
            'col-input-number',
        ),
    );
    $tableHeader[] = array(
        'data' => '<div id="grand-total-2" class="align-right"></div>',
        'class' => array(
            'col-input-number',
        ),
    );
    return $tableHeader;
}
function setoran_form_row($idpremis = null, $month = null, $year = null){
    $tableRow = array();
    $tableRow['#tree'] = TRUE;
    $dataSetoran = get_setoran_premis_data($idpremis, $month, $year);
    if (!empty($idpremis)){
        $lastDate = get_last_day($month,$year);
        $intDate = mktime(7,0,0,$month,1,$year);
        $startDate = date('Y-m-',$intDate).'01';
        $finishDate = date('Y-m-',$intDate).$lastDate;
        $dateArray = create_date_range_array($startDate,$finishDate, 'UNIX');
        $dateArray2 = create_date_range_array($startDate,$finishDate, 'd-m-Y');
        for ($i = 0;$i < count($dateArray);$i++){
            if (isset($dataSetoran[$dateArray[$i]])){
                $idSetoran = $dataSetoran[$dateArray[$i]]->id;
                $description = $dataSetoran[$dateArray[$i]]->description;
                //$jumlah = $dataSetoran[$dateArray[$i]]->jumlah;
                $jumlah_entry = $dataSetoran[$dateArray[$i]]->jumlah_entry;
            }else{
                $idSetoran = null;
                $description = t('SETORAN');
                //$jumlah = 0;
                $jumlah_entry = 0;
            }
            $rowData = array();
            if (!empty($idSetoran)){
                $idxDate = 'date-'.$idpremis.'-'.$dateArray[$i].'-'.($i + 1).'-'.$idSetoran;
            }else{
                $idxDate = 'date-'.$idpremis.'-'.$dateArray[$i].'-'.($i + 1);
            }
            $rowData[$idxDate] = array(
                '#type' => 'item', '#title' => $dateArray2[$i],
                '#attributes' => array('outerclass' => array('align-center')),
            );
            if (!empty($idSetoran)){
                $idxDesc = 'description-'.$idpremis.'-'.$dateArray[$i].'-'.($i + 1).'-'.$idSetoran;
            }else{
                $idxDesc = 'description-'.$idpremis.'-'.$dateArray[$i].'-'.($i + 1);
            }
            $rowData[$idxDesc] = array(
                '#type' => 'textfield',
                '#default_value' => $description,
                '#attributes' => array('outerclass' => array('align-left')),
            );
            if (!empty($idSetoran)){
                $idxJumlah = 'jumlah-'.$idpremis.'-'.$dateArray[$i].'-'.($i + 1).'-'.$idSetoran;
            }else{
                $idxJumlah = 'jumlah-'.$idpremis.'-'.$dateArray[$i].'-'.($i + 1);
            }
            $rowData[$idxJumlah] = array(
                '#type' => 'textfield',
                '#default_value' => $jumlah_entry,
                '#attributes' => array('class' => array(
                        'align-right','setoran-value','setoran-'.($i + 1)
                    ),
                ),
            );
            if (!empty($idSetoran)){
                $idxTotal = 'total-'.$dateArray[$i].'-'.($i + 1).'-'.$idSetoran;
            }else{
                $idxTotal = 'total-'.$dateArray[$i].'-'.($i + 1);
            }
            $rowData[$idxTotal] = array(
                '#type' => 'item', '#title' => '<div id="total-'.($i + 1).'">TOTAL</div>',
                '#attributes' => array('outerclass' => array('align-right')),
            );
            $tableRow['r'.($i + 1)] = $rowData;
        }
    }
    return $tableRow;
}
function setoran_form($form, &$form_state, $variables = null){
    set_default_time_zone();
    if (isset($variables['id_premis']) && !empty($variables['id_premis'])){
        $dataPremis = get_data_premis_by_id(null, $variables['id_premis']);
        $month = $variables['month'];
        $year = $variables['year'];
        if (!empty($month) && !empty($year)){
        }else{
            $month = date('n');
            $year = date('Y');
        }
        $intDate = mktime(7,0,0,$month,1,$year);
        $tableHeader = setoran_form_header();
        $tableFooter[] = setoran_form_footer();
        $tableRow = setoran_form_row($variables['id_premis'], $month, $year);
        $form['form-title'] = array(
            '#type' => 'item',
            '#markup' => '<h3 class="title">OVERHEAD DAN SERAHAN '.$dataPremis->nama.' '.date('M',$intDate).' '.date('Y',$intDate).'</h3>',
        );
        $form['id_premis'] = array(
            '#type' => 'hidden',
            '#default_value' => $variables['id_premis'],
        );
        $form['table'] = array(
            '#theme' => 'formtable_form_table',
            '#header' => $tableHeader,
            'rows' => $tableRow,
            '#footer' => $tableFooter,
        );
        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Save Setoran'),
        );
    }
    return $form;
}
function setoran_form_submit($form, &$form_state)
{
    global $user;
    set_default_time_zone();
    if (isset($form_state['values']['rows'])) {
        $values = $form_state['values']['rows'];
    } else {
        $values = $form_state['values'];
    }
    $returnData = array();
    if (count($values)) {
        $inserted = 0;
        $updated = 0;
        foreach ($values as $idxRow => $rowData){
            if (count($rowData)){
                $savedData = array();
                foreach ($rowData as $idxCol => $colData){
                    $explodeIdx = explode('-',$idxCol);
                    if (isset($explodeIdx[4])){
                        $savedData['id'] =  $explodeIdx[4];
                    }
                    $savedData['id_premis'] = $explodeIdx[1];
                    $savedData['setoran_date'] = $explodeIdx[2];
                    $savedData['month'] = date('n',$explodeIdx[2]);
                    $savedData['year'] = date('Y',$explodeIdx[2]);
                    $savedData['uid'] = $user->uid;
                    if ($explodeIdx[0] == 'jumlah'){
                        $savedValue = str_replace('.','',$colData);
                        $savedData[$explodeIdx[0]] = eval('return '.$savedValue.';');
                        $savedData['jumlah_entry'] = $colData;
                    }else{
                        $savedValue = $colData;
                        $savedData[$explodeIdx[0]] = $savedValue;
                    }
                }
                if (isset($savedData['id']) && !empty($savedData['id'])){
                    //Update record
                    $returnData[] = saveTableData('zpremis_setoran','admin data setoran',$savedData,'id',$savedData['id']);
                    $updated++;
                }else{
                    //Insert record
                    $returnData[] = saveTableData('zpremis_setoran','admin data setoran',$savedData,'id');
                    $inserted++;
                }
            }
        }
        drupal_set_message(t('Data setoran berhasil di save...!!!'));
    }
}

function get_setoran_premis_data($idPremis = null, $month = null, $year = null, $idSetoran = null){
    $tableName = 'zpremis_setoran';
    $fieldArray = array(
        'id', 'id_premis', 'setoran_date', 'jumlah', 'jumlah_entry',
        'description', 'month', 'year', 'created', 'changed', 'uid', 'uid_changed'
    );
    $fieldOrder = array(
        array ('fieldname' => 'id_premis'),
        array('fieldname' => 'setoran_date'),
    );
    $dataSetoran = array();
    if (empty($idSetoran)) {
        $conditionArray = array();
        if (!empty($idPremis)) {
            $consData = new stdClass();
            $consData->fieldName = 'ta.id_premis';
            $consData->value = $idPremis;
        }
        if (!empty($month) && !empty($year)) {
            $consData = new stdClass();
            $consData->fieldName = 'ta.month';
            $consData->value = $month;
            $conditionArray[] = $consData;
            $consData = new stdClass();
            $consData->fieldName = 'ta.year';
            $consData->value = $year;
            $conditionArray[] = $consData;
        }
        $setoranPremis = getTableDataByField(
            $tableName,'access data setoran',$fieldArray,
            $fieldOrder,null,null,null,$conditionArray
        );
        if (!empty($idPremis)) {
            if (count($setoranPremis)){
                for ($i = 0;$i < count($setoranPremis);$i++){
                    $dataSetoran[$setoranPremis[$i]->setoran_date] = $setoranPremis[$i];
                }
            }
        }else{
            if (count($setoranPremis)){
                for ($i = 0;$i < count($setoranPremis);$i++){
                    $dataSetoran[$setoranPremis[$i]->id_premis][] = $setoranPremis[$i];
                }
            }
        }
    }else{
        $setoranPremis = getTableDataByField(
            $tableName,'access data setoran',$fieldArray,
            $fieldOrder,'id',$idSetoran
        );
        $dataSetoran = $setoranPremis[0];
    }
    return $dataSetoran;
}