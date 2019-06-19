<?php

class Par3_Controller_Dashboard
{
    public $model;

    public function __construct($inuid = null, $obrid = null)
    {
        $this->model = new Par3_Model_Obra();
        $this->model->carregarPorId($obrid);
    }

    public function getCoordenadasObras($dados = array())
    {
        $where = array();
        if (!empty($dados)) {

            if (!empty($dados['estuf'])) {
                $dados['estuf'] = array_filter($dados['estuf']);
                if ($dados['estuf']) {
                    $strUf = implode('\',\'', $dados['estuf']);
                    $where[] = " i.estuf IN ('{$strUf}') ";
                }
            }

            if (!empty($dados['muncod'])) {
                $dados['muncod'] = array_filter($dados['muncod']);
                if ($dados['muncod']) {
                    $strMuncod = implode('\',\'', $dados['muncod']);
                    $where[] = " i.muncod IN ('{$strMuncod}') ";
                }
            }

            if (!empty($dados['esdid'])) {
                $dados['esdid'] = array_filter($dados['esdid']);
                if ($dados['esdid']) {
                    $strEsdid = implode('\',\'', $dados['esdid']);
                    $where[] = " esd.esdid IN ('{$strEsdid}') ";
                }
            }
            if (!empty($dados['obrid'])) {
                $where[] = " obrid = {$dados['obrid']} ";
            }
            if (!empty($dados['preid'])) {
                $where[] = " preid = {$dados['preid']}";
            }
        }
        if (!empty($where))
            $strWhere = ' AND ' . implode(' AND ', $where);
        $sql = "SELECT 
                    obrdsc, obrlatitude, obrlongitude,
                    CASE WHEN oct.octid = 3 THEN 'R'
                        WHEN oct.octid = 2 THEN 'A'
                        WHEN oct.octid = 1 THEN 'C'
                        ELSE NULL
                    END as categoria
                FROM par3.obra o
                INNER JOIN par3.instrumentounidade i ON i.inuid = o.inuid
                INNER JOIN par3.obra_tipo otp ON otp.otpid = o.otpid
                INNER JOIN par3.obra_categoria oct on oct.octid = otp.octid
                INNER JOIN workflow.documento doc ON doc.docid = o.docid
                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid and esd.tpdid = 304
                WHERE obrstatus = 'A'  {$strWhere} ";
        $coordenadas = $this->model->carregar($sql);
//
//        ver($sql, d);
        if ($coordenadas)
            return $this->tratarCoordenadas($coordenadas);
        else
            return array();
    }

    private function tratarCoordenadas($coordenadas)
    {
        $coordenadasTratadas = array();

        foreach ($coordenadas as $coors) {
            if ($this->isGeoValid('longitude', $coors['obrlongitude'])) {
                if (strpos($coors['obrlatitude'], 'º') === false
                    && strpos($coors['obrlatitude'], '°') === false
                    && strpos($coors['obrlatitude'], '\"') === false
                    && strpos($coors['obrlatitude'], '.') !== false
                ) {
                    $coordenadasTratadas[] = $coors;
                }
            }
        }
        return $coordenadasTratadas;
    }

    public function isGeoValid($type, $value)
    {
        $pattern = ($type == 'latitude')
            ? '/^(\+|-)?(?:90(?:(?:\.0{1,8})?)|(?:[0-9]|[1-8][0-9])(?:(?:\.[0-9]{1,8})?))$/'
            : '/^(\+|-)?(?:180(?:(?:\.0{1,8})?)|(?:[0-9]|[1-9][0-9]|1[0-7][0-9])(?:(?:\.[0-9]{1,8})?))$/';

        return preg_match($pattern, $value);
    }

    function DecimalToDMS($decimal, &$degrees, &$minutes, &$seconds, &$direction, $type = true) {
        //set default values for variables passed by reference
        $degrees = 0;
        $minutes = 0;
        $seconds = 0;
        $direction = 'X';

        //decimal must be integer or float no larger than 180;
        //type must be Boolean
        if(!is_numeric($decimal) || abs($decimal) > 180 || !is_bool($type)) {
            return false;
        }

        //inputs OK, proceed
        //type is latitude when true, longitude when false

        //set direction; north assumed
        if($type && $decimal < 0) {
            $direction = 'S';
        }
        elseif(!$type && $decimal < 0) {
            $direction = 'W';
        }
        elseif(!$type) {
            $direction = 'E';
        }
        else {
            $direction = 'N';
        }

        //get absolute value of decimal
        $d = abs($decimal);

        //get degrees
        $degrees = floor($d);

        //get seconds
        $seconds = ($d - $degrees) * 3600;

        //get minutes
        $minutes = floor($seconds / 60);

        //reset seconds
        $seconds = floor($seconds - ($minutes * 60));
    }

}

?>