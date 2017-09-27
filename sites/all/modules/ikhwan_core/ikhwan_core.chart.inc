<?php

function assignment_all_3d_column_chart($month = null, $year = null, $zone = null)
{
    $detect = mobile_detect_get_object();
    $assignmentTotal = '';
    $getDataZone = getZoneByCode();
    $namaZone = array();
    if (count($getDataZone)) {
        for ($i = 0; $i < count($getDataZone); $i++) {
            $namaZone[$getDataZone[$i]->zone] = $getDataZone[$i]->nama;
        }
    }
    $strSQL = 'SELECT asdt.assignment_name, asz.zone, asz.assignment_id, ';
    $strSQL .= 'sum(asz.bank_in) AS total_bank_in FROM {zassigment_zone} AS asz ';
    $strSQL .= 'LEFT JOIN {zdata_assigment} asdt ON asz.assignment_id = asdt.id ';
    $strSQL .= 'WHERE asz.month = :month AND asz.year = :year ';
    $strSQL .= 'GROUP BY asz.zone,asz.assignment_id ORDER BY asz.zone,asz.assignment_id';
    $arrValue = array(':month' => $month, ':year' => $year);
    $arrAssignment = db_query($strSQL, $arrValue)->fetchAll();
    $assignmentData = array();
    $intDate = mktime(7, 0, 0, $month, 1, $year);
    $assignmentData['title'] = 'Assignment Zon ' . date('M Y', $intDate);
    $assignmentName = array();
    $seriesData = array();
    $zoneData = array();
    $totalAssignmentVal = 0;
    for ($i = 0; $i < count($arrAssignment); $i++) {
        if ($i == 0){
            $zoneData[] = $arrAssignment[$i]->zone;
            $assignmentData['categories'][] = $namaZone[$arrAssignment[$i]->zone];
        }else{
            if (!in_array($arrAssignment[$i]->zone,$zoneData)){
                $zoneData[] = $arrAssignment[$i]->zone;
                $assignmentData['categories'][] = $namaZone[$arrAssignment[$i]->zone];
            }
        }
        $seriesData[$arrAssignment[$i]->assignment_id][] = (float)$arrAssignment[$i]->total_bank_in;
        $totalAssignmentVal = $totalAssignmentVal + $arrAssignment[$i]->total_bank_in;
        $assignmentName[$arrAssignment[$i]->assignment_id] = $arrAssignment[$i]->assignment_name;
    }
    $assignmentData['subtitle'] = 'Total Assignment : RM ' . number_format($totalAssignmentVal,0,'.',',');
    if (count($seriesData)){
        foreach ($seriesData as $assKey => $assData){
            $assignmentData['series'][] = (object)array(
                'name' => $assignmentName[$assKey],
                'data' => $assData,
            );
        }
    }
    $options = assignment_all_3d_column_stacked_chart_options($assignmentData);
    if (is_object($options)) {
        // Optionally add styles or any other valid attributes, suitable for
        // drupal_attributes().
        if ($detect->isMobile()){
            $attributes = array('style' => array('height: 450px;'));
        }else {
            $attributes = array('style' => array('height: 600px;'));
        }
        // Return block definition.
        $assignmentTotal = highcharts_render($options, $attributes);
    }
    return $assignmentTotal;
}

function assignment_total_3d_column_chart($assignmentId = null, $month = null, $year = null, $zone = null, $seriesColor = null){
    $assignmentTotal = '';
    if (!empty($assignmentId)) {
        $jenisAssignment = get_assignment_by_id($assignmentId);
        $getDataZone = getZoneByCode();
        $namaZone = array();
        if (count($getDataZone)){
            for ($i = 0;$i < count($getDataZone);$i++){
                $namaZone[$getDataZone[$i]->zone] = $getDataZone[$i]->nama;
            }
        }
        $strSQL = 'SELECT sum(bank_in) AS total_bank_in, zone FROM {zassigment_zone} ';
        $strSQL .= 'WHERE assignment_id = :assignment_id AND month = :month AND year = :year ';
        $strSQL .= 'GROUP BY zone ORDER BY total_bank_in DESC';
        $arrValue = array(':assignment_id' => $assignmentId, ':month' => $month, ':year' => $year);
        $arrAssignment = db_query($strSQL,$arrValue)->fetchAll();
        $assignmentData = array();
        $seriesData = array();
        $intDate = mktime(7,0,0,$month,1,$year);
        $assignmentData['title'] = $jenisAssignment->assignment_name.' '.date('M Y', $intDate);
        $totalAssignment = 0;
        for ($i = 0;$i < count($arrAssignment);$i++){
            $assignmentData['categories'][] = $namaZone[$arrAssignment[$i]->zone];
            $seriesData[] = (float)$arrAssignment[$i]->total_bank_in;
            $totalAssignment = $totalAssignment + $arrAssignment[$i]->total_bank_in;
        }
        $assignmentData['subtitle'] = 'Total '.$jenisAssignment->assignment_name.' : RM '.number_format($totalAssignment,0,'.',',');
        $assignmentData['series'] = (object)array(
            'name' => $jenisAssignment->assignment_name,
            'data' => $seriesData,
        );
        $options = assignment_harian_3d_column_chart_options($assignmentData, $seriesColor);
        if (is_object($options)) {
            // Optionally add styles or any other valid attributes, suitable for
            // drupal_attributes().
            $attributes = array('style' => array('height: 450px;'));

            // Return block definition.
            $assignmentTotal = highcharts_render($options, $attributes);
        }
    }
    return $assignmentTotal;
}

function assignment_harian_3d_column_chart_options($assignmentData = null, $seriesColor = null){
    $options = null;
    if (!empty($assignmentData) && isset($assignmentData['categories']) && !empty($assignmentData['categories'])) {
        $options = new stdClass();
        // Chart.
        $options->chart = (object)array(
            'renderTo' => 'container',
            'type' => 'column',
            'backgroundColor' => 'transparent',
            'marginTop' => '30',
            'options3d' => (object)array(
                'enabled' => TRUE,
                'alpha' => 15,
                'beta' => 15,
                'viewDistance' => 25,
                'depth' => 60,
            ),
        );
        // Title.
        $options->title = new stdClass();
        $options->title->text = strtoupper($assignmentData['title']);

        // Sub Title
        $options->subtitle = new stdClass();
        $options->subtitle->text = strtoupper($assignmentData['subtitle']);

        // Plot options.
        $options->plotOptions = new stdClass();
        $options->plotOptions->column = (object)array(
            'depth' => 35,
        );
        if (!empty($seriesColor)){
            $options->plotOptions->series = (object)array(
                'color' => $seriesColor,
            );
        }
        //X-axis.
        $options->xAxis = new stdClass();
        $options->xAxis = (object)array(
            'categories' => $assignmentData['categories'],
        );

        //Y-axis
        $options->yAxis = new stdClass();
        $options->yAxis = (object)array(
            'title' => (object)array(
                'text' => 'Total Bank In',
             ),
        );

        // Legend.
        $options->legend = new stdClass();
        $options->legend = (object)array(
            'enabled' => false,
        );

        // Series.
        $options->series = array();
        $series = new StdClass();
        $series->name = $assignmentData['series']->name;
        $series->data = $assignmentData['series']->data;
        /*$series->dataLabels = (object)array(
            'enabled' => true,
            'rotation' => -90,
            'color' => '#FFFFFF',
            'align' => 'right',
            'format' => '{point.y}',
            'y' => 10, // 10 pixels down from the top
            'style' => (object)array(
                'fontSize' => '13px',
                'fontFamily' => 'Verdana, sans-serif',
            ),
        );*/
        $options->series[] = $series;
        // Disable credits.
        $options->credits = new stdClass();
        $options->credits->enabled = FALSE;
    }
    return $options;
}
function assignment_all_3d_column_stacked_chart_options($assignmentData = null){
    $options = null;
    if (!empty($assignmentData) && isset($assignmentData['categories']) && !empty($assignmentData['categories'])) {
        $options = new stdClass();
        // Chart.
        $options->chart = (object)array(
            'renderTo' => 'container',
            'type' => 'column',
            'backgroundColor' => 'transparent',
            'marginTop' => '10',
            'marginBottom' => '110',
            'options3d' => (object)array(
                'enabled' => TRUE,
                'alpha' => 15,
                'beta' => 15,
                'viewDistance' => 25,
                'depth' => 60,
            ),
        );
        // Title.
        $options->title = new stdClass();
        $options->title->text = strtoupper($assignmentData['title']);

        // Sub Title
        $options->subtitle = new stdClass();
        $options->subtitle->text = strtoupper($assignmentData['subtitle']);

        // Plot options.
        $options->plotOptions = new stdClass();
        $options->plotOptions->column = (object)array(
            'depth' => 35,
            'stacking' => 'normal',
        );

        //X-axis.
        $options->xAxis = new stdClass();
        $options->xAxis = (object)array(
            'categories' => $assignmentData['categories'],
        );

        //Y-axis
        $options->yAxis = new stdClass();
        $options->yAxis = (object)array(
            'title' => (object)array(
                'text' => 'Total Bank In',
            ),
        );

        // Legend.
        $options->legend = new stdClass();
        $options->legend = (object)array(
            'align' => 'right',
            'x' => -30,
            'verticalAlign' => 'top',
            'y' => 75,
            'floating' => true,
            'backgroundColor' => 'transparent',
            'borderColor' => '#CCC',
            'borderWidth' => 1,
            'shadow' => false,
        );

        // Tooltip
        $options->tooltip = new stdClass();
        $options->tooltip = (object)array(
            'headerFormat' => '<b>{point.x}</b><br/>',
            'pointFormat' => '{series.name}: {point.y}<br/>Total Assignment: {point.stackTotal}',
        );

        // Series.
        $options->series = array();
        $options->series = $assignmentData['series'];
        // Disable credits.
        $options->credits = new stdClass();
        $options->credits->enabled = FALSE;
    }
    return $options;
}
function overhead_total_3d_column_chart($month = null, $year = null, $zone = null, $seriesColor = null)
{
    $ohTotal = '';
    $getDataZone = getZoneByCode();
    $namaZone = array();
    if (count($getDataZone)) {
        for ($i = 0; $i < count($getDataZone); $i++) {
            $namaZone[$getDataZone[$i]->zone] = $getDataZone[$i]->nama;
        }
    }
    $arrOverhead = array();
    if (function_exists('get_total_overhead_by_zone_converted')){
        $arrOverhead = get_total_overhead_by_zone_converted($month, $year, $zone);
    }
    $overheadData = array();
    $seriesData = array();
    $intDate = mktime(7, 0, 0, $month, 1, $year);
    $totalOverhead = 0;
    if (count($arrOverhead)) {
        foreach ($arrOverhead as $idxZone => $ohTotal) {
            $overheadData['categories'][] = $namaZone[$idxZone];
            $seriesData[] = (float)$ohTotal;
            $totalOverhead = $totalOverhead + (float)$ohTotal;
        }
    }
    $overheadData['title'] = 'OVERHEAD ZON ' . date('M Y', $intDate);
    $overheadData['subtitle'] = 'TOTAL OVERHEAD : RM. '.number_format($totalOverhead,0,',','.');
    $overheadData['series'] = (object)array(
        'name' => t('OVERHEAD'),
        'data' => $seriesData,
    );
    $options = overhead_total_3d_column_chart_options($overheadData, $seriesColor);
    if (is_object($options)) {
        // Optionally add styles or any other valid attributes, suitable for
        // drupal_attributes().
        $attributes = array('style' => array('height: 450px;'));

        // Return block definition.
        $overheadTotal = highcharts_render($options, $attributes);
    }
    return $overheadTotal;
}

function overhead_total_3d_column_chart_options($assignmentData = null, $seriesColor = null){
    $options = null;
    if (!empty($assignmentData)) {
        $options = new stdClass();
        // Chart.
        $options->chart = (object)array(
            'renderTo' => 'container',
            'type' => 'column',
            'backgroundColor' => 'transparent',
            'marginTop' => '30',
            'options3d' => (object)array(
                'enabled' => TRUE,
                'alpha' => 15,
                'beta' => 15,
                'viewDistance' => 25,
                'depth' => 60,
            ),
        );
        // Title.
        $options->title = new stdClass();
        $options->title->text = strtoupper($assignmentData['title']);
        $options->subtitle = new stdClass();
        $options->subtitle->text = strtoupper($assignmentData['subtitle']);
        // Plot options.
        $options->plotOptions = new stdClass();
        $options->plotOptions->column = (object)array(
            'depth' => 35,
        );
        if (!empty($seriesColor)){
            $options->plotOptions->series = (object)array(
                'color' => $seriesColor,
            );
        }
        //X-axis.
        $options->xAxis = new stdClass();
        $options->xAxis = (object)array(
            'categories' => $assignmentData['categories'],
        );

        //Y-axis
        $options->yAxis = new stdClass();
        $options->yAxis = (object)array(
            'title' => (object)array(
                'text' => 'Total OH ('.variable_get('google_currency_converter_to', '').')',
            ),
        );

        // Legend.
        $options->legend = new stdClass();
        $options->legend = (object)array(
            'enabled' => false,
        );

        // Series.
        $options->series = array();
        $series = new StdClass();
        $series->name = $assignmentData['series']->name;
        $series->data = $assignmentData['series']->data;
        /*$series->dataLabels = (object)array(
            'enabled' => true,
            'rotation' => -90,
            'color' => '#FFFFFF',
            'align' => 'right',
            'format' => '{point.y}',
            'y' => 10, // 10 pixels down from the top
            'style' => (object)array(
                'fontSize' => '13px',
                'fontFamily' => 'Verdana, sans-serif',
            ),
        );*/
        $options->series[] = $series;
        // Disable credits.
        $options->credits = new stdClass();
        $options->credits->enabled = FALSE;
    }
    return $options;
}

function cash_flow_zon_chart($month = null, $year = null, $zone = null){
    $detect = mobile_detect_get_object();
    $assignmentTotal = '';
    $getDataZone = getZoneByCode();
    $namaZone = array();
    if (count($getDataZone)) {
        for ($i = 0; $i < count($getDataZone); $i++) {
            $namaZone[$getDataZone[$i]->zone] = $getDataZone[$i]->nama;
        }
    }
    $strSQL = 'SELECT zone, SUM(jumlah_pemasukan) AS total_pemasukan, SUM(jumlah_pengeluaran) AS total_pengeluaran ';
    $strSQL .= 'FROM {zdatazone_pemasukan_pengeluaran} ';
    $strSQL .= 'WHERE month = :month AND year = :year GROUP BY zone ';
    $strSQL .= 'ORDER BY total_pemasukan DESC, total_pengeluaran DESC';
    $arrValue = array(':month' => $month, ':year' => $year);
    $arrCashflow = db_query($strSQL, $arrValue)->fetchAll();
    $cashflowData = array();
    $intDate = mktime(7, 0, 0, $month, 1, $year);
    $cashflowData['title'] = 'Cashflow Zon ' . date('M Y', $intDate);
    $seriesData = array();
    $zoneData = array();
    $totalPemasukan = 0;
    $totalPengeluaran = 0;
    for ($i = 0; $i < count($arrCashflow); $i++) {
        if ($i == 0){
            $zoneData[] = $arrCashflow[$i]->zone;
            $cashflowData['categories'][] = $namaZone[$arrCashflow[$i]->zone];
        }else{
            if (!in_array($arrCashflow[$i]->zone,$zoneData)){
                $zoneData[] = $arrCashflow[$i]->zone;
                $cashflowData['categories'][] = $namaZone[$arrCashflow[$i]->zone];
            }
        }
        $seriesData['Pemasukan'][] = (float)$arrCashflow[$i]->total_pemasukan;
        $seriesData['Pengeluaran'][] = (float)$arrCashflow[$i]->total_pengeluaran;
        $totalPemasukan = $totalPemasukan + $arrCashflow[$i]->total_pemasukan;
        $totalPengeluaran = $totalPengeluaran + $arrCashflow[$i]->total_pengeluaran;
    }
    $totalBaki = $totalPemasukan - $totalPengeluaran;
    $cashflowData['subtitle'] = 'Total Pemasukan : RM ' . number_format($totalPemasukan,0,'.',',');
    $cashflowData['subtitle'] .= ' Total Pengeluaran : RM '.number_format($totalPengeluaran,0,'.',',');
    $cashflowData['subtitle'] .= ' Total Baki : RM '.number_format($totalBaki,0,'.',',');
    if (count($seriesData)){
        foreach ($seriesData as $assKey => $assData){
            $cashflowData['series'][] = (object)array(
                'name' => $assKey,
                'data' => $assData,
            );
        }
    }
    $options = cash_flow_zon_chart_options($cashflowData);
    if (is_object($options)) {
        // Optionally add styles or any other valid attributes, suitable for
        // drupal_attributes().
        if ($detect->isMobile()){
            $attributes = array('style' => array('height: 450px;'));
        }else {
            $attributes = array('style' => array('height: 600px;'));
        }
        // Return block definition.
        $cashFlowChart = highcharts_render($options, $attributes);
    }
    return $cashFlowChart;
}

function cash_flow_zon_chart_options($cashflowData = null, $seriesColor = null){
    $options = null;
    if (!empty($cashflowData) && isset($cashflowData['categories']) && !empty($cashflowData['categories'])) {
        $options = new stdClass();
        // Chart.
        $options->chart = (object)array(
            'renderTo' => 'container',
            'type' => 'column',
            'backgroundColor' => 'transparent',
            'marginTop' => '10',
            'marginBottom' => '110',
            'options3d' => (object)array(
                'enabled' => TRUE,
                'alpha' => 15,
                'beta' => 15,
                'viewDistance' => 25,
                'depth' => 60,
            ),
        );
        // Title.
        $options->title = new stdClass();
        $options->title->text = strtoupper($cashflowData['title']);

        // Sub Title
        $options->subtitle = new stdClass();
        $options->subtitle->text = strtoupper($cashflowData['subtitle']);

        //X-axis.
        $options->xAxis = new stdClass();
        $options->xAxis = (object)array(
            'categories' => $cashflowData['categories'],
        );

        //Y-axis
        $options->yAxis = new stdClass();
        $options->yAxis = (object)array(
            'title' => (object)array(
                'text' => 'Total',
            ),
        );

        // Legend.
        $options->legend = new stdClass();
        $options->legend = (object)array(
            'align' => 'right',
            'x' => -30,
            'verticalAlign' => 'top',
            'y' => 75,
            'floating' => true,
            'backgroundColor' => 'transparent',
            'borderColor' => '#CCC',
            'borderWidth' => 1,
            'shadow' => false,
        );

        // Tooltip
        $options->tooltip = new stdClass();
        $options->tooltip = (object)array(
            'headerFormat' => '<b>{point.x}</b><br/>',
            'pointFormat' => '{series.name}: {point.y}<br/>Total Baki: {point.stackTotal}',
        );

        // Series.
        $options->series = array();
        $options->series = $cashflowData['series'];
        // Disable credits.
        $options->credits = new stdClass();
        $options->credits->enabled = FALSE;
    }
    return $options;
}
