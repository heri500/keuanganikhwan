<?php

function assignment_all_3d_column_chart($month = null, $year = null, $zone = null)
{
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
    $seriesData = array();
    $intDate = mktime(7, 0, 0, $month, 1, $year);
    $assignmentData['title'] = 'Assignment Zon ' . date('M Y', $intDate);
    $assignmentName = array();
    $seriesData = array();
    $zoneData = array();
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
        $assignmentName[$arrAssignment[$i]->assignment_id] = $arrAssignment[$i]->assignment_name;
    }
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
        $attributes = array('style' => array('height: 700px;'));

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
        for ($i = 0;$i < count($arrAssignment);$i++){
            $assignmentData['categories'][] = $namaZone[$arrAssignment[$i]->zone];
            $seriesData[] = (float)$arrAssignment[$i]->total_bank_in;
        }
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
    if (!empty($assignmentData)) {
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
            'y' => 25,
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