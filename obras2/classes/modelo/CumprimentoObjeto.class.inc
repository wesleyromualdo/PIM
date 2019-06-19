<?php
/**
 * Description of CumprimentoObjeto
 *
 * @author lindalbertofilho
 */
class CumprimentoObjeto extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.cumprimento_objeto";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("coid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'coid' => null,
        'obrid' => null,
        'docid' => null,
        'alerta' => null,
        'dtverificacao' => null,
        'status' => null
    );

    /**
     * Obras permitidas.
     * @param type $obrid
     */
    public function obrasPermitidas($obrid) {
        $sql = "select * FROM obras2.controle_cumprimento_objeto cco WHERE ccostatus = 'A' AND obrid = $obrid";
        $data = $this->pegaLinha($sql);
        return $data ? true : false;
        #$permitidas = array(21263,21196,21267,21277,21295,21279,21274,21151,21255,21219,21228,21227,21170,21294,21159,21221,21256,21174,21254,21165,21223,21179,21224,21197,21298,21264,21181,21164,21296,21253,21217,21290,21287,21257,21209,21273,21234,21269,21283,21289,21280,21275,21262,21265,21176,21286,21270,21154,21285,21299,21250,21266,21211,21291,21199,21293,21292,21149,21233,21260,21237,21226,21167,21169,21218,21160,21278,21276,21259,21222,21268,21271,21272,21225,21297,21213,21168,21288,21080,12465,12466,12467,12468,12469,12470,12471,12472,12473,12474,12475,12476,12477,12478,12481,12482,12483,12484,12485,12486,12487,12488,12489,12490,12491,12492,12493,12494,12495,12496,12498,12499,12500,12501,12502,12503,12504,12505,12506,12507,12508,12509,12510,12511,12512,12514,12516,12517,12518,12519,12521,12522,12523,12524,12525,12526,12527,12528,12529,12530,12531,12533,12534,12535,12536,12537,12538,12539,12540,12541,12543,12544,12545,18076,7893,25163,18193,25397,25739,25398,13339,18096,17448,1640,8726,8692,4438,19447,13289,1953,24718,8801,25503,25166,8411,17952,14327,8715,18082,19361,8390,19177,24578,13248,1867,8573,13288,2148,11561,8487,1572,1414,18907,8708,2030,20142,1368,8521,1415,13636,2059,8636,8418,1796,8412,18984,8758,8668,8824,3551,8379,8954,8501,20021,3770,11841,1952,1581,3800,2068,12617,8685,2768,1723,8772,13275,8400,24682,8377,1566,1652,13359,25142,19275,24681,19856,24573,8569,8645,2001,13368,11731,25149,1458,20134,2137,1630,19985,11788,2102,19948,8682,4284,2188,8564,8779,1537,8611,13351,18939,1370,8610,25574,25634,18906,2088,2207,25716,19539,25392,18075,1720,2158,19990,18912,2182,25539,8742,19721,8803,2033,1809,8409,13474,8516,2168,19772,8543,11727,8670,19189,13366,8429,2176,25143,1959,13372,4474,24286,8548,1486,11829,11825,25482,8792,24683,12614,17536,8776,25707,14636,14808,25391,2232,1578,4291,20148,19021,18472,2036,11808,1741,13252,1739,11686,8401,25459,19687,25468,17513,11871,12606,1397,25687,19869,20189,3576,8856,25770,8590,8509,2136,11796,2119,25386,17449,8667,8626,20266,27722,25460,8381,20188,24950,1777,4437,20146,1689,1541,8802,12648,1682,1464,25400,19894,8606,25390,8488,19441,8753,25402,13081,20158,2241,19400,20088,13542,8890,17501,8605,18999,17446,1419,7897,25740,11814,19989,1444,11836,14594,7905,8524,11720,11810,25579,25461,1516,1616,19402,8635,14603,25158,1432,23235,8784,25728,24542,18087,25389,20073,19520,20227,24384,19980,8493,25401,1435,19401,1579,25387,23769,13358,8471,2107,2078,11907,19051,8532,8760,19190,7883,3764,25462,19358,8868,19255,20211,13297,1386,1635,1711,20125,20159,20263,17953,2120,23270,8843,19028,2431,13291,8380,8555,8520,25738,19759,25465,8674,1742,1922,1438,8434,1810,8882,8596,1667,1632,7891,1478,2089,2131,1916,2066,8683,1818,1807,8559,8830,1562,1461,1872,1898,1858,8796,8937,3785,8356,2133,1881,3826,1993,14618,7913,8858,1980,1568,8870,8935,1729,7896,14615,1849,1825,1646,2021,1933,1955,1470,1721,8396,8451,8442,1709,1615,1418,8992,1619,8888,1987,8687,1921,1811,2081,1639,2106,8942,1896,8686,1460,1732,8423,1764,2103,14789,1984,1830,2218,2134,8463,1488,12763,1986,3827,8860,1505,27604,1991,1527,8978,1594,2150,1525,9647,1802,1902,1411,8912,1629,1782,8756,1753,1670,8713,1479,1449,13690,2113,1917,1873,8871,8444,8825,8427,3767,3803,1508,1862,8838,8897,2172,3795,3763,8542,8899,1988,2108,8919,1889,13109,1666,8365,1544,1776,1417,9434,8528,8795,1694,1963,8450,8867,1671,1967,1895,2130,1761,1376,2077,1771,1749,2045,1557,1812,2669,1766,1633,8405,1859,8952,2013,1680,1702,8722,8981,8994,1974,8991,8903,12950,8966,8673,1842,7758,8866,1861,1710,1770,7918,2026,1556,12365,7889,8389,1822,8999,4631,1805,8432,13985,1643,2090,8368,1999,3809,1591,1725,1780,1501,8926,2005,1852,3787,2171,8459,9404,1906,2135,1961,2079,1759,2865,1375,1611,1990,1584,2116,1684,1905,1561,13687,2072,3794,1778,7892,8823,1487,2147,2031,2145,1853,2854,2040,1932,1760,1595,8735,1482,1416,1374,1377,1407,1429,1462,1485,1528,1601,1621,1634,1669,1934,2025,2048,2209,3779,4281,8669,8773,11792,13234,13235,13236,13244,13352,14814,14818,17477,17605,17937,17938,17943,17949,17951,17991,18060,18061,18070,19256,19757,20162,23771,24860,25164,25173,25301,25474,25475,25476,25477,25478,25479,1004661,1006785,25388,7179,7182,7184,13364,7180,7185,7187,7181);
        #return in_array($obrid, $permitidas);
    }

    public function dataLiberacaoCumprimentoObjeto($arrayObras) {
       if($arrayObras) {
           $sql = "select  to_char(dtcriacao, 'dd/mm/yyyy') as dtAberturaCumpObj
                        FROM obras2.controle_cumprimento_objeto cco
                     WHERE ccostatus = 'A' AND obrid in ({$arrayObras}) order by dtcriacao asc limit 1";
           $data = $this->pegaLinha($sql);
       }
        return $data;
    }

    public function verificaExistencia($obrid)
    {
 


        $sql = <<<DML
            SELECT coid, obrid, docid, status FROM {$this->stNomeTabela} WHERE obrid = $obrid;
DML;
        $dados = $this->pegaLinha($sql);
 
        $this->popularDadosObjeto($dados);
        if(!$dados || $dados['status'] == 'I') {
            $docid = wf_cadastrarDocumento(200, "Fluxo de cumprimento de objeto");
            $this->popularDadosObjeto(array('obrid' => $obrid, 'docid' => $docid, 'status' => 'A', 'alerta' => 'A'));
            $this->salvar();
            $this->commit();
        }
        if(!$this->docid) {
            $docid = wf_cadastrarDocumento(200, "Fluxo de cumprimento de objeto");
            $this->popularDadosObjeto(array('docid' => $docid));
            $this->salvar();
            $this->commit();
        }
        return $this->coid;
    }

    public function verificaEstado($endid)
    {
        $sql = <<<DML
            select estuf from territorios.municipio where muncod = (select muncod from entidade.endereco where endid = $endid)
DML;
        return $this->pegaUm($sql);
    }

    public function pegaPorDocid($docid)
    {
        $sql = <<<DML
            SELECT coid, obrid, docid FROM {$this->stNomeTabela} WHERE docid = $docid;
DML;
        $dados = $this->pegaLinha($sql);
        $this->popularDadosObjeto($dados);
        return $dados;
    }

    public function tramitaConclusao($params)
    {
        $sql = <<<DML
            SELECT docid FROM {$this->stNomeTabela} WHERE coid = {$params['coid']};
DML;
        $docid = $this->pegaUm($sql);
        $dados = wf_pegarDocumento($docid);

        if(wf_acaoPossivel($docid, $params['estado'], array())) {
            $acao = wf_pegarAcao($dados['esdid'], $params['estado']);
            wf_alterarEstado($dados['docid'], $acao['aedid'], '', array('coid' => $this->coid));
        }
    }

    public function criaSubAba($url, $perfilHabilitado = false, $obrid, $esdid)
	{
        $inacabadaCancelada = false;
        #Exceï¿½ï¿½o para Obras Inacabadas e Canceladas.
        $esdidObra = $this->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid FROM obras2.obras WHERE obrid = $this->obrid)");
        #Nï¿½o se aplica para obra cancelada.
        if($esdidObra == ESDID_OBJ_CANCELADO) $inacabadaCancelada = true;
        #Nï¿½o se aplica para obra inacabada e ultima vistoria incompleta.
        if($esdidObra == ESDID_OBJ_INACABADA) {
            $sql = <<<DML
                SELECT
                    s.staid
                FROM obras2.supervisao s
                WHERE s.obrid = '$obrid'
                    AND s.emsid IS NULL
                    AND s.smiid IS NULL
                    AND s.supstatus = 'A'
                    AND validadaPeloSupervisorUnidade = 'S'
                ORDER BY s.supdata DESC, s.supdtinclusao DESC
                LIMIT 1
DML;
            $staid = $this->pegaUm($sql);
            if($staid != 3) $inacabadaCancelada = true;
        }
		$where = "";
		if($_SESSION['sisid']){
			$where = " and menu.sisid = {$_SESSION['sisid']} ";
		}
        $rs = array(
            array('mnulink' => 'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A','mnutransacao' => 'Documentacao', 'mnudsc' => 'Documentação'),
            array('mnulink' => 'obras2.php?modulo=principal/cadQuestionarioCumprimentoObjeto&acao=A','mnutransacao' => 'Questionario', 'mnudsc' => 'Questionário'),
            array('mnulink' => 'obras2.php?modulo=principal/cadDiligenciaCumprimentoObjeto&acao=A','mnutransacao' => 'Diligencia', 'mnudsc' => 'Diligência'),
            array('mnulink' => 'obras2.php?modulo=principal/cadConclusaoCumprimentoObjeto&acao=A','mnutransacao' => 'Conclusao', 'mnudsc' => 'Conclusão da Obra'),
            array('mnulink' => 'obras2.php?modulo=principal/cadProcessoCumprimentoObjeto&acao=A','mnutransacao' => 'Conclusao', 'mnudsc' => 'Resultado da Análise ')
        ); ?>
        <div class="row">
            <br>
            <div class="col-md-12">
                <ul class="nav nav-tabs">
                    <?php
                    foreach($rs as $key => $tab):
                        if(!possui_perfil(array(PFLCOD_SUPER_USUARIO,PFLCOD_GESTOR_MEC)) || IS_PRODUCAO) {
                        	
                            if ($key == 2 && ($esdid == ESDID_CUMPRIMENTO_CADASTRAMENTO || !$perfilHabilitado)) {
                            	if(!possui_perfil(array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_UNIDADE)))
                            	{	
                            		continue;
                            	}
                            }
                            if($key == 3 && ($esdid == ESDID_CUMPRIMENTO_CADASTRAMENTO || $esdid == ESDID_CUMPRIMENTO_DILIGENCIADO || !$perfilHabilitado)) 
                            { //
                            	continue;
                            }
                            if($key == 3 && possui_perfil(array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_UNIDADE)))
                            {
                            	continue;
                            }
                            if($key == 4 && ($esdid == ESDID_CUMPRIMENTO_CADASTRAMENTO || !$perfilHabilitado)) 
                            { //
                            	if(!possui_perfil(array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_UNIDADE)))
                            	{
                            		continue;
                            	}
                            }

                            /* OS 5701
                             * Adicionada o if temporarioamente pela os 5701
                             */
                            if($key == 1 && $inacabadaCancelada) {
                            continue;
                            }
                        }
                        /* OS 5701
                         * Adicionado comentario no if temporarioamente pela os 5701
                         */
                        //if($key == 1 && $inacabadaCancelada) {
                        //    continue;
                        //}
                        ?>
                        <li <?php if($tab['mnulink'] == $url) echo 'class="active"' ?>><a id="tab<?php echo str_replace(' ' , '' , $tab['mnutransacao']) ?>" href="<?php echo $tab['mnulink']?>"><?php echo $tab['mnudsc'] ?></a></li>
                    <?php
                    endforeach; ?>
                </ul>
            </div>
        </div>
        <br>
        <?php
	}

    public function dadosTramite()
    {
        if(!$this->docid) {
            return false;
        }
        $sql = <<<DML
            SELECT
                ed.esddsc AS estado_anterior,
                ed2.esdid,
                ed2.esddsc AS estado_atual,
                ac.aeddscrealizada AS acao,
                us.usuemail,
                us.usunome,
                TO_CHAR(hd.htddata,'DD/MM/YYYY') AS htddata,
                cd.cmddsc
            FROM workflow.documento d
            INNER JOIN workflow.historicodocumento hd ON (d.hstid = hd.hstid)
            INNER JOIN workflow.acaoestadodoc ac ON ac.aedid = hd.aedid
            INNER JOIN workflow.estadodocumento ed ON ed.esdid = ac.esdidorigem
            INNER JOIN workflow.estadodocumento ed2 ON ed2.esdid = ac.esdiddestino
            INNER JOIN seguranca.usuario us ON us.usucpf = hd.usucpf
            LEFT JOIN workflow.comentariodocumento cd ON cd.hstid = hd.hstid
            WHERE hd.docid = {$this->docid}
DML;
        return $this->pegaLinha($sql);
    }

    public function dadosTramiteEsdid()
    {
        if(!$this->docid) {
            return false;
        }
        $sql = <<<DML
            SELECT
                ed.esdid
            FROM workflow.documento d
            INNER JOIN workflow.historicodocumento hd ON (d.hstid = hd.hstid)
            INNER JOIN workflow.acaoestadodoc ac ON ac.aedid = hd.aedid
            INNER JOIN workflow.estadodocumento ed ON ed.esdid = ac.esdidorigem
            WHERE hd.docid = {$this->docid}
DML;
        return $this->pegaLinha($sql);
    }

    public function alertaInicio()
    {
        if(possui_perfil(array(PFLCOD_GESTOR_UNIDADE))) {
            #Gestor Unidade
            $sql = "
                SELECT obr.obrid FROM obras2.obras obr
                WHERE obr.entid IN (
                    SELECT urs.entid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A'
                        AND urs.usucpf = '" . $_SESSION['usucpf'] . "'
                        AND urs.pflcod = '".PFLCOD_GESTOR_UNIDADE."'
                )";
            $dados = $this->carregar($sql);
        } else if(possui_perfil(array(PFLCOD_SUPERVISOR_UNIDADE))) {
            #Fiscal Unidade
            $usuarioResp = new UsuarioResponsabilidade();
            $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
            $arEmpid = ($arEmpid ? $arEmpid : array(0));
            $sql = "
                SELECT obrid FROM obras2.obras WHERE empid IN (
                    SELECT urs.empid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A'
                        AND urs.usucpf = '" . $_SESSION['usucpf'] . "'
                        AND urs.pflcod = '" .PFLCOD_SUPERVISOR_UNIDADE. "'
                        AND urs.empid IN('" . implode("', '", $arEmpid) . "'))";
            $dados = $this->carregar($sql);
        }
        if($dados) {
            $obras = array();
            foreach($dados as $linha) {
                $obras[] = $linha['obrid'];
            }
            $obras = implode(", ", $obras);
            $sql = "SELECT co.coid, co.obrid, o.obrnome FROM {$this->stNomeTabela} co INNER JOIN obras2.obras o ON (co.obrid = o.obrid AND o.obrstatus = 'A' AND o.obridpai IS NULL) WHERE co.obrid IN ($obras) AND co.alerta = 'A' AND co.status = 'A'";
            $saida = $this->carregar($sql);
        }
        return $saida;
    }

    public function marcarAlertaLido($coids, $obrids, $obrnomes)
    {
        if(count($coids) > 0) {
            foreach($coids as $key => $coid) {
                $this->coid = $coid;
                $this->alerta = 'V';
                $this->salvar();
                $obrids[$key];
                $sql = <<<DML
                    INSERT INTO obras2.registroatividade (obrid,usucpf,rgadscsimplificada,rgadsccompleta,rgaautomatica,traid)
                    VALUES({$obrids[$key]},'{$_SESSION['usucpf']}','Confirmado a leitura do informativo de abertura do cumprimento do objeto.','Confirmado a leitura pelo usuario {$_SESSION['usunome']} do informativo de abertura do cumprimento do objeto da obra ({$obrids[$key]}) {$obrnomes[$key]}.','t',1);
DML;
                $this->executar($sql);
            }
            $this->commit();
        }
    }

    public function retornaNumeroProcesso()
    {
        $sql = "SELECT pronumeroprocesso FROM obras2.vm_termo_convenio_obras WHERE obrid = {$this->obrid}";
        return $this->pegaUm($sql);
    }


    /**
     * Cancela o acesso da obra a aba cumprimento o objeto, envia o e-mail e cadastra a obra no alerta
     * @param $obrid
     */
    public function cancelaCumprimentoObra($obrid)
    {
        $cco = new ControleCumprimentoObjeto();
        $alerta = new AlertaObra();
        if($cco->verificaObra($obrid)){
            // Remoce o acesso a aba
            $cco->removerObra($obrid);

            // Remove o alerta do cumprimento do objeto
            $alerta->removeAlerta($obrid, 1);

            // Envia o e-mail
            $email = new Email();
            $email->enviaEmailCancelamentoCumprimento($obrid);

        } else {
            return false;
        }
    }

    /**
     * Libera o acesso da obra a aba cumprimento o objeto, envia o e-mail e cadastra a obra no alerta
     * @param $obrid
     */
    public function liberaCumprimentoObra($obrid, $enviaEmail=true)
    {
        $cco = new ControleCumprimentoObjeto();
        if(!$cco->verificaObra($obrid)){
            // Libera o acesso a aba
            $cco->inserirObra($obrid);

            if ($enviaEmail) {
                // Envia o e-mail
                $email = new Email();
                $email->enviaEmailObraConcluida($obrid);

                // Cadastra o Alerta
                $alerta = new AlertaObra();
                $alerta->novo($obrid, 1);
            }

        } else {
            return false;
        }
    }

    /**
     * Bloqueia o acesso da obra a aba cumprimento o objeto, envia o e-mail e cadastra a obra no alerta
     * @param $obrid
     */
    public function bloqueiaCumprimentoObra($obrid)
    {
        $cco = new ControleCumprimentoObjeto();
        $teste = $cco->verificaObra($obrid);

        if($teste){
            // Bloqueia o acesso a aba
            $sql = "UPDATE obras2.controle_cumprimento_objeto SET ccostatus = 'I'
                    WHERE obrid = $obrid";

            $this->carregar($sql);
            $this->commit();

            // remove o alarta ua
            $alerta = new AlertaObra();
            $alerta->removeAlerta($obrid, 1);

            return true;
        } else {
            return false;
        }
    }

    public function verificaRestricoesDiligencia($coid)
    {
        $this->carregarPorId($coid);

        $restriçõesSql = "
                            SELECT
                              r.rstid, r.docid, d.esdid
                            FROM obras2.restricao r
                            JOIN workflow.documento d ON d.docid = r.docid
                            WHERE
                                r.obrid = {$this->obrid}
                                AND r.rstdsc = 'Cumprimento do Objeto Diligênciado.'
                                AND r.fsrid = 7
                                AND d.esdid IN (".ESDID_AGUARDANDO_PROVIDENCIA.", ".ESDID_AGUARDANDO_CORRECAO.", ".ESDID_AGUARDANDO_ANALISE_FNDE.");";
        $restricoes = $this->carregar($restriçõesSql);

        if(!empty($restricoes)){
            foreach($restricoes as $restricao){
                $acao = wf_pegarAcao($restricao['esdid'], ESDID_SUPERADA);
                wf_alterarEstado($restricao['docid'], $acao['aedid'], 'Cumprimento do objeto saiu da diligência.', array('rstid' => $restricao['rstid'], 'obrid' =>$this->obrid, 'coid' => $coid));
            }
        }
    }
}