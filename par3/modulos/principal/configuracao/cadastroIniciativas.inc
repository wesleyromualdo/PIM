<?php
//die('teste');
require_once APPRAIZ . "includes/funcoesspo_componentes.php";
// include_once APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';
include_once APPRAIZ . 'par3/classes/controller/Iniciativa.class.inc';
include_once APPRAIZ . 'par3/classes/model/Iniciativa.class.inc';

$controllerIniciativa = new Par3_Controller_Iniciativa();
$modelIniciativa = new Par3_Model_Iniciativa();

// Edição: Obtem dados da iniciativa caso exista o seu id.
$iniid = validarInteiro($_REQUEST['iniid'])? $_REQUEST['iniid']:'';
$iniciativa = array();
if($iniid){
    $iniciativa = $modelIniciativa->getIniciativa(array('iniid'=>$iniid));
}

$modelIniciativaDescricao = new Par3_Model_IniciativaDescricao();
$rsIniciativaDescricao = $modelIniciativaDescricao->pegarSQLSelectCombo();

$modelIniciativaTiposAtendimento = new Par3_Model_IniciativaTiposAtendimento();
$rsIniciativaTipoAtendimento = $modelIniciativaTiposAtendimento->pegarSQLSelectCombo();

$modelCicloPar = new Par3_Model_CicloPar();
$rsCicloPar = $modelCicloPar->pegarSQLSelectCombo();

$modelIniciativaAnos = new Par3_Model_IniciativaIniciativasAnos();
$rsIniciativaAnos = array();
$arrIniciativaAnosSalvo = array();
//print_r($modelIniciativaAnos->recuperarTodos('*', array("iniid = 1"), null)); exit();

$modelIniciativaTiposObjeto= new Par3_Model_IniciativaTiposObjeto();
$rsIniciativaTiposObjeto = $modelIniciativaTiposObjeto->pegarSQLSelectCombo();

$modelIniAreaRelPrg         = new Par3_Model_IniciativaAreasRelacionadasProgramas;
$modelIniciativaProgramas   = new Par3_Model_IniciativaIniciativasProgramas();
$modelPrograma              = new Par3_Model_Programa();
$sqlProgramasPossiveis      = $modelPrograma->pegarSQLSelectCombo(null, array('incluirSigla'=>true));

$modelIniciativaAreas= new Par3_Model_IniciativaAreas();
$arrIniciativaAreasSalvas = array();

$modelIniciativaIniciativasAreas = new Par3_Model_IniciativaIniciativasAreas();

$modelProjeto= new Par3_Model_Projeto();
$rsProjeto = $modelProjeto->pegarSQLSelectCombo();
$arrProjetosSalvos = array();

$modelIniciativaIniciativasProjetos = new Par3_Model_IniciativaIniciativasProjetos();

$modelEstado = new Par3_Model_Estado();
$whereEstado = '';

if($iniciativa){
    $whereEstado = $iniciativa['iniesfera'] == 'M' ? " WHERE estcod != '53' " : '';
}

$rsEstado =
    "SELECT estuf as codigo, estdescricao as descricao FROM(
			SELECT
				'T' as estuf,
				'Todos' as estdescricao
			UNION
			SELECT
				estuf AS codigo,
				estdescricao AS descricao
			FROM
				territorios.estado
				{$whereEstado}
		) estado
			ORDER BY
				CASE WHEN estuf = 'T' THEN 1 END,
				estuf";

$arrEstadosSalvos = array();

$modelIniciativaEstados = new Par3_Model_IniciativaIniciativasEstados();

$modelGrupotipomunicipio = new Par3_Model_Grupotipomunicipio();
$rsGrupotipomunicipio = $modelGrupotipomunicipio->pegarSQLSelectCombo();
$arrGtMunicipiosSalvos = array();

$modelIniciativaGrupoTipoMunicipio = new Par3_Model_IniciativaIniciativasGrupoTipoMunicipio();

// Armanezará os programas possíveis ao relacionar com a IniciativaÁrea da Iniciativa a ser editada.
$arrProgramasSalvos = array();

$modelIniciativaMunicipios = new Par3_Model_IniciativaIniciativasMunicipios();
$modelMunicipio = new Par3_Model_Municipio();
$sqlMunicipiosPossíveis = array();
$arrMunicipiosSalvos = array();

//Funções
// Transforma o campo array ou string separado por vírgula em strings com valores entre áspas simples e separados por vírgula.
function toStringQuotationMarks($campos){
    $campos = is_array($campos) ? $campos : explode(",", $campos);
    if(count($campos) == 1){
        $resultado = "'$campos[0]'";
    } else {
        $listaCampo = array();
        foreach($campos as $c){
            array_push($listaCampo, "'$c'");
        }
        $resultado = implode(",", $listaCampo);
    }

    return $resultado;
}


switch ($_REQUEST ['req']) {
    case 'salvar':
        ob_clean();
        $resp = $controllerIniciativa->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'listarAnos' :
        ob_clean ();
        if ($_REQUEST ['cicid']) {
            $sql = $modelIniciativaAnos->pegarSQLSelectCombo(null, $_REQUEST ['cicid']);
            if($sql){
                $json = simec_json_encode($modelIniciativaAnos->carregar($sql));
            }else{
                $json = '{}';
            }
        }else{
            $json = '{}';
        }
        echo $json;
        die;
        break;
    case 'listarAreasProgramas' :
        ob_clean ();
        if ($_REQUEST ['prgid']) {
            $sql = $modelIniAreaRelPrg->pegarSQLSelectComboAreasPorPrograma($_REQUEST ['prgid']);
            if($sql){
                $json = simec_json_encode($modelIniAreaRelPrg->carregar($sql));
            }else{
                $json = '{}';
            }
        }else{
            $json = '{}';
        }
        echo $json;
        die;
        break;
    case 'listarMunicipios':
        ob_clean ();
        if ($_REQUEST['estuf']) {
            $estuf = $_REQUEST ['estuf'];
            $estuf = $estuf == 'T' ? null : $estuf;
            $opcoesPegarSQLComboMunicipios	= array('incluirOpcaoTodos'=>true);
            $sql = $modelMunicipio->pegarSQLSelectCombo(null, $estuf, $opcoesPegarSQLComboMunicipios);
            if($sql){
                $json = simec_json_encode($modelMunicipio->carregar($sql));
            }else{
                $json = '{}';
            }
        }else{
            $json = '{}';
        }
        echo $json;
        die;
        break;
    case 'verificaIniciativaProinfancia':
        ob_clean();
        echo $controllerIniciativa->verificaIniciativaProinfancia();
        die;
        break;
}
// Caso seja edição:
if($iniid){
    //Campo Anos.
    // Edição: Obtém os anos do ciclo já existente para o caso de existência do id da iniciaitva (edição).
    if($iniciativa['cicid']){
        // Obtem os valores possíveis.
        $rsIniciativaAnos = $modelIniciativaAnos->pegarSQLSelectCombo(null, $iniciativa['cicid']);
        // Obtem os valores salvos na iniciativa.
        $rsIniciativaAnosSalvo = $modelIniciativaAnos->pegarSQLSelectCombo($iniciativa['iniid'], null);
        //print_r($rsIniciativaAnosSalvo); exit();
        $anosIniciativaSalvos = $modelIniciativaAnos->carregar($rsIniciativaAnosSalvo);
        // Transforma o resultado em array com os valores dos códigos dos anos.
        if($anosIniciativaSalvos){
            foreach($anosIniciativaSalvos as $ano){
                array_push($arrIniciativaAnosSalvo, $ano['codigo']);
            }
        }
        //print_r($arrIniciativaAnosSalvo); exit();
    }
    // Campo Área Relacionada:
    // Cria o array dos ids dos Programas da Iniciativa de edição.
    $programasSalvos = $modelIniciativaProgramas->recuperarTodos('prgid', array("iniid = $iniid AND iiprgstatus = 'A'"), null);
    // Alimenta o array com os ids dos Programas da Iniciativa:
    foreach($programasSalvos as $programa){
        array_push($arrProgramasSalvos, $programa['prgid']);
    }
    // Obtem os valores das IniciativasÁreas salvas na iniciativa.
    $areasSalvas = $modelIniciativaIniciativasAreas->recuperarTodos('iarid', array("iniid = $iniid AND iiarstatus = 'A'"), null);
    // Transforma o resultado em array com os valores dos códigos das IniciativasÁreas
    foreach($areasSalvas as $area){
        array_push($arrIniciativaAreasSalvas, $area['iarid']);
    }
    // Campo Programa:
    // Obtem os valores Possíveis que depende da Área Relacionada selecionada.
    if($arrProgramasSalvos){
        $wherePrograma = implode(', ', $arrProgramasSalvos);
        $rsAreasPossiveis = $modelIniAreaRelPrg->recuperarTodos('iarid', array("prgid IN ($wherePrograma)"), null);
        $arrAreasPossiveis = array();
        // Cria um array com os ids dos programas possíveis a partir das IniciativasÁreas da iniciativa retornada na edição.
        foreach($rsAreasPossiveis as $area){
            array_push($arrAreasPossiveis, $area['iarid']);
        }
        // Obtém os SQL do combobox dos Programas possíveis relacionados às ÁreasIniciativas da iniciativa de edição:
        $rsIniciativaAreas = $arrAreasPossiveis ? $modelIniciativaAreas->pegarSQLSelectComboDescricao(array('iarid' => $arrAreasPossiveis)) : array();
    }
    // Campo Projeto.
    // Obtem os valores dos Projetos salvos na iniciativa.
    $projetosSalvos = $modelIniciativaIniciativasProjetos->recuperarTodos('proid', array("iniid = $iniid AND iiprostatus = 'A'"), null);
    // Transforma o resultado em array com os valores dos códigos dos projetos da iniciativa.
    foreach($projetosSalvos as $projeto){
        array_push($arrProjetosSalvos, $projeto['proid']);
    }
    // Campo Estados.
    // Obtem os valores dos Estados salvos na iniciativa.
    $estadosSalvos = $modelIniciativaEstados->recuperarTodos('estuf', array("iniid = $iniid AND iiestufstatus = 'A'"), null);
    // Se existir o valor T (Todos) no campo "initodos_estuf" da iniciativa, insere nos estados salvos ($arrEstadosSalvos).
    if($iniciativa['initodos_estuf'] == 'S'){
        array_push($estadosSalvos, array('estuf'=>'T'));
    }
    // Transforma o resultado em array com os valores dos códigos dos estados da iniciativa.
    foreach($estadosSalvos as $estado){
        array_push($arrEstadosSalvos, $estado['estuf']);
    }

    // Campo Grupo Tipo Município.
    // Obtem os valores dos Grupos Tipo de Municpípio salvos na iniciativa.
    $gtMunicipiosSalvos = $modelIniciativaGrupoTipoMunicipio->recuperarTodos('gtmid', array("iniid = $iniid AND iigtmstatus = 'A'"), null);
    // Transforma o resultado em array com os valores dos códigos dos estados da iniciativa.
    foreach($gtMunicipiosSalvos as $gtMunicipio){
        array_push($arrGtMunicipiosSalvos, $gtMunicipio['gtmid']);
    }

    // Campo Município:
    // Obtem os valores Possíveis que depende do Estado selecionado.
    if($arrEstadosSalvos){
        $whereMunicipio = toStringQuotationMarks($arrEstadosSalvos);
        $rsMunicipiosPossiveis = $modelMunicipio->recuperarTodos('muncod', array("estuf IN ($whereMunicipio)"), null);
        $arrMunicipiosPossiveis = array();
        // Cria um array com os ids dos municipios possíveis a partir dos Estados da iniciativa retornada na edição.
        foreach($rsMunicipiosPossiveis as $municipio){
            array_push($arrMunicipiosPossiveis, $municipio['muncod']);
        }
        // Obtém os SQL do combobox dos Municipios possíveis relacionados aos Estados da iniciativa de edição:
        $arrMunicipiosPossiveis = toStringQuotationMarks($arrMunicipiosPossiveis);
        //print_r($arrMunicipiosPossiveis); exit();
        $opcoesPegarSQLComboMunicipios	= array('incluirOpcaoTodos'=>true);
        $sqlMunicipiosPossíveis = $arrMunicipiosPossiveis ? $modelMunicipio->pegarSQLSelectCombo($arrMunicipiosPossiveis, array(), $opcoesPegarSQLComboMunicipios) : array();
    }
    // Cria o array dos ids dos Municipios da Iniciativa de edição.
    $municipiosSalvos = $modelIniciativaMunicipios->recuperarTodos('muncod', array("iniid = $iniid AND iimunidstatus ='A'"), null);

    // Alimenta o array com os ids dos Municípios da Iniciativa:
    foreach($municipiosSalvos as $municipio){
        array_push($arrMunicipiosSalvos, $municipio['muncod']);
    }
    // Se existir o valor T (Todos) no campo "initodos_muncod" da iniciativa, insere nos municípios salvos ($arrMunicipiosSalvos)
    if($iniciativa['initodos_muncod'] == 'S'){
        array_push($arrMunicipiosSalvos, 'T');
    }
    // Se exisitr o valor T (todos) no campo do Estado ("initodos_estuf") da iniciativa, busca todos os municípios como possíveis ($sqlMunicipiosPossiveis).
    if($iniciativa['initodos_estuf'] == 'S'){
        $sqlMunicipiosPossíveis = $modelMunicipio->pegarSQLSelectCombo(null, null, array('incluirOpcaoTodos'=>true));
    }

}

?>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <!-- <input type="hidden" id="itrid" name="itrid" value="<?php //echo $itrid; ?>"> -->
                <h3 class="center">Incluir Iniciativa</h3>
            </div>
        </div>
        <div class="ibox-content">
            <form method="post" name="formIniciativa" id="formIniciativa" class="form form-horizontal">
                <div class="row">
                    <div class="col-md-offset-3 col-md-6 center">
                        <?php
                        echo $simec->input ( 'iniid', 'Código', $iniciativa['iniid'] ? $iniciativa['iniid'] : null, array (
                            'placeHolder' => '',
                            'disabled' => true
                        ), array ('input-size' => '2')
                        );

                        echo $simec->select ( 'indid', 'Descrição Iniciativa', $iniciativa['indid'] ? $iniciativa['indid'] : null, $rsIniciativaDescricao, array (
                            'placeHolder' => '',
                            'maxlengh' => 50,
                            $boIndicador,
                            'required'
                        ) );
                        echo $simec->select ( 'intaid', 'Tipo de Atendimento', $iniciativa['intaid'] ? $iniciativa['intaid'] : null, $rsIniciativaTipoAtendimento, array (
                            'placeHolder' => 'Tipo de Atendimento',
                            'maxlengh' => 50,
                            'required'
                        ) );

                        echo $simec->select ( 'cicid', 'Ciclo', $iniciativa['cicid'] ? $iniciativa['cicid'] : null, $rsCicloPar, array (
                            'placeHolder' => 'Ciclo',
                            'maxlengh' => 50,
                            'required'
                        ) );

                        echo $simec->select ( 'iniano', 'Ano', $arrIniciativaAnosSalvo, $rsIniciativaAnos, array (
                            'placeHolder' => 'Ciclo',
                            'maxlengh' => 50,
                            'required','multiple'
                        ) );
                        // Edição:
                        //if($rsIniciativaAnos){
                        //echo '<div class="form-group"><label class="col-lg-3 control-label">Ano: *</label><div class="col-sm-9 col-md-9 col-lg-9 ">';
                        //echo inputCombo("iniano[]", $rsIniciativaAnos, $arrIniciativaAnosSalvo,'iniano', array('multiple'=> 'multiple', 'titulo'=>'Selecione'));
                        //echo '</div></div>';
                        // Criação:
                        /*}else{
                            echo $simec->select ( 'iniano[]', 'Ano', array (), array(), array (
                                    'placeHolder' => 'Ano',
                                    'maxlengh' => 50,
                                    $disabled,
                                    'required'
                            ) );
                        }*/

                        echo $simec->select ( 'intoid', 'Tipo de Objeto', $iniciativa['intoid'] ? $iniciativa['intoid'] : null, $rsIniciativaTiposObjeto, array (
                            'placeHolder' => 'Objeto Iniciativa',
                            'maxlengh' => 50,
                            'required'
                        ) );

                        echo $simec->select ( 'prgid', 'Programa', $arrProgramasSalvos, $sqlProgramasPossiveis, array (
                            'placeHolder' => 'Ciclo',
                            'maxlengh' => 50,
                            'required','multiple'
                        ) );

                        //echo '<div class="form-group"><label class="col-lg-3 control-label">Programa: *</label><div class="col-sm-9 col-md-9 col-lg-9 ">';
                        //echo inputCombo("prgid[]", $sqlProgramasPossiveis, $arrProgramasSalvos, 'prgid', array('multiple'=> 'multiple', 'titulo'=>'Selecione'));
                        //echo '</div></div>';
                        /*echo $simec->select ( 'prgid[]', 'Programa', array (), array(), array (
                                'placeHolder' => 'Ano',
                                'maxlengh' => 50,
                                $disabled,
                                'required'
                        ) );*/

                        $rsIniciativaAreas = $rsIniciativaAreas ? $rsIniciativaAreas : array();
                        echo $simec->select ( 'iarid', 'Área Relacionada', $arrIniciativaAreasSalvas, $rsIniciativaAreas, array (
                            'placeHolder' => 'Ciclo',
                            'maxlengh' => 50,
                            'required','multiple'
                        ) );

                        //if($rsIniciativaAnos){

                        //echo '<div class="form-group"><label class="col-lg-3 control-label">Área Relacionada: *</label><div class="col-sm-9 col-md-9 col-lg-9 ">';
                        //echo inputCombo("iarid[]", $rsIniciativaAreas, $arrIniciativaAreasSalvas,'iarid', array('multiple'=> 'multiple', 'titulo'=>'Selecione'));
                        //echo '</div></div>';
                        // Criação:
                        //}else{
                        /*echo $simec->select ( 'iarid[]', 'Área Relacionada', $dados ['iarid'], $rsIniciativaAreas, array (
                                'placeHolder' => 'Areas Relacionadas',
                                'maxlengh' => 50,
                                'required'
                        ) );*/
                        //}

                        echo $simec->select ( 'proid', 'Projeto', $arrProjetosSalvos, $rsProjeto, array (
                            'placeHolder' => 'Ciclo',
                            'maxlengh' => 50,'multiple'
                        ) );

                        //echo '<div class="form-group"><label class="col-lg-3 control-label">Projeto:</label><div class="col-sm-9 col-md-9 col-lg-9 ">';
                        //echo inputCombo("proid[]", $rsProjeto, $arrProjetosSalvos, 'proid', array('multiple'=> 'multiple', 'titulo'=>'Selecione'));
                        //echo '</div></div>';

                        /*echo $simec->select ( 'proid[]', 'Projeto', array (), $rsProjeto, array (
                                'placeHolder' => 'Ano',
                                'maxlengh' => 50,
                                $disabled,
                        ) );*/

                        $rsEsfera = array(
                            'M' => 'Municipal',
                            'E' => 'Estadual',
                            'T' => 'Todos'
                        );

                        echo $simec->select ( 'iniesfera', 'Esfera', $iniciativa ? $iniciativa['iniesfera'] : array(), $rsEsfera, array (
                            'placeHolder' => 'Esfera',
                            'maxlengh' => 50,
                            $disabled ,
                            'required'
                        ) );

                        echo $simec->select ( 'estuf', 'Estados', $arrEstadosSalvos, $rsEstado, array (
                            'placeHolder' => 'Ciclo',
                            'maxlengh' => 50,
                            'required','multiple'
                        ) );

                        //echo '<div class="form-group"><label class="col-lg-3 control-label">Estados: *</label><div class="col-sm-9 col-md-9 col-lg-9 ">';
                        //echo inputCombo("estuf[]", $rsEstado, $arrEstadosSalvos, 'estuf', array('multiple'=> 'multiple', 'titulo'=>'Selecione'));
                        //echo '</div></div>';

                        /*echo $simec->select ( 'estuf[]', 'Estados', array (), $rsEstado, array (
                                'placeHolder' => 'Estados',
                                'maxlengh' => 50,
                                $disabled ,
                                'required'
                        ) );*/
                        // Município

                        echo $simec->select ( 'muncod', 'Municipio', $arrMunicipiosSalvos, $sqlMunicipiosPossíveis, array (
                            'placeHolder' => 'Ciclo',
                            'maxlengh' => 50,
                            'required','multiple'
                        ) );

                        //echo "<div id='form-group-municipio'>";
                        //echo '<div class="form-group"><label class="col-lg-3 control-label"> Municipio: *</label><div class="col-sm-9 col-md-9 col-lg-9 ">';
                        //echo inputCombo("muncod[]", $sqlMunicipiosPossíveis, $arrMunicipiosSalvos, 'muncod', array('multiple'=> 'multiple', 'titulo'=>'Selecione'));
                        //echo '</div></div>';
                        //echo "</div>";

                        // Grupo Tipo Município
                        echo $simec->select ( 'gtmid', 'Grupo de Municipios', $arrGtMunicipiosSalvos, $rsGrupotipomunicipio, array (
                            'placeHolder' => 'Ciclo',
                            'maxlengh' => 50,
                            'multiple'
                        ) );

                        //echo "<div id='form-group-grupo-tipo-municipio'>";
                        //echo '<div class="form-group"><label class="col-lg-3 control-label">Grupo de Municipios: </label><div class="col-sm-9 col-md-9 col-lg-9 ">';
                        //echo inputCombo("gtmid[]", $rsGrupotipomunicipio, $arrGtMunicipiosSalvos, 'gtmid', array('multiple'=> 'multiple', 'titulo'=>'Selecione'));
                        //echo '</div></div>';
                        //echo "</div>";
                        $tiposA = array (
                            "A" => "Ativo",
                            "I" => "Inativo"
                        );
                        ?>

                        <div class="form-group">
                            <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Situação: *</label>
                            <div class="col-sm-9 col-md-9 col-lg-3 ">
                                <div id="inisituacao" style="text-align: left;">
                                    <div class="radio radio-default radio-inline ">
                                        <input type='radio' checked name='inisituacao' id='inisituacao1' value='A'" <?php echo $iniciativa['inisituacao'] == "A" ? " checked" : ""; ?>>
                                        <label for="inisituacao1">Ativo</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="inisituacao" id="inisituacao2" value="I" <?php echo $iniciativa['inisituacao'] == "I" ? " checked" : ""; ?>>
                                        <label for="inisituacao2">Inativo</label>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="form-group ">
		                    <label class="col-sm-3 col-md-3 col-lg-3 control-label  control-label">Validar Pendência: * </label>
		                    <div class="col-sm-9 col-md-9 col-lg-9 ">
		                        <div id="inivalidapendencia" style="text-align: left;">
		                        	<div class="radio radio-default radio-inline ">
		                            	<input type='radio' checked name='inivalidapendencia' id='inivalidapendencia1' value='S'" <?php echo $iniciativa['inivalidapendencia'] == "S" ? " checked" : ""; ?>>
                                        <label for="inivalidapendencia1">Sim</label>
									</div>
									<div class="radio radio-default radio-inline ">
										<input type="radio" name="inivalidapendencia" id="inivalidapendencia2" value="N" <?php echo $iniciativa['inivalidapendencia'] == "N" ? " checked" : ""; ?>>
                                        <label for="inivalidapendencia2">Não</label>
									</div>
								</div>
		                    </div>
		                </div>
                        <div class="form-group ">
		                    <label class="col-sm-3 col-md-3 col-lg-3 control-label  control-label">Obras Proinfância: * </label>
		                    <div class="col-sm-9 col-md-9 col-lg-9 ">
		                        <div id="inivalidapendencia" style="text-align: left;">
		                        	<div class="radio radio-default radio-inline ">
		                            	<input type='radio' checked name='iniobraproinfancia' id='iniobraproinfancia1' value='true'" <?php echo $iniciativa['iniobraproinfancia'] == "t" ? " checked" : ""; ?>>
                                        <label for="iniobraproinfancia1">Sim</label>
									</div>
									<div class="radio radio-default radio-inline ">
										<input type="radio" name="iniobraproinfancia" id="iniobraproinfancia2" value="false" <?php echo $iniciativa['iniobraproinfancia'] <> 't' ? " checked" : ""; ?>>
                                        <label for="iniobraproinfancia2">Não</label>
									</div>
								</div>
		                    </div>
		                </div>
                    </div>
                </div>
                <div class="ibox-footer">
                    <div class="center">
                        <button id="cancelarIniciativa" class="btn btn-default">
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="submit" id="salvarIniciativa" class="btn btn-success">
                            <i class="fa fa-plus-square-o"></i> Salvar
                        </button>
                        <button type="button" id="salvarSeguirProximo" class="btn btn-primary xls" style="width: 110px;">
                            <i class="fa fa-mail-forward"></i> Próximo
                        </button>
                    </div>
                </div>
            </form>
        </div>
    </div>
</div>
<div id="modal-form" class="modal fade" aria-hidden="true">
    <div class="modal-dialog">
        <div class="ibox-title">
            <h5 id="html_modal-title"></h5>
            <button type="button" class="close" data-dismiss="modal"
                    aria-label="Close">
                <span aria-hidden="true">&times;</span>
            </button>
        </div>
        <div id="html_modal-form"></div>
    </div>

    <div id="modal-form-large" class="modal fade" aria-hidden="true">
        <div class="ibox-title">
            <h5 id="html_modal-title-large"></h5>
            <button type="button" class="close" data-dismiss="modal"
                    aria-label="Close">
                <span aria-hidden="true">&times;</span>
            </button>
        </div>
        <div id="html_modal-form-large"></div>
    </div>
</div>

<script>
    var option = '<option value label selected=selected ></option>';

    /*function removerOpcaoSelecione(){

     }*/

    $('[name="iniobraproinfancia"]').on('change',function(){
        if($(this).attr('id') == 'iniobraproinfancia1') {
            $.post(
                window.location,
                {'req': 'verificaIniciativaProinfancia'},
                function (iniid) {
                    if (trim(iniid) > 0) {
                        swal('Alerta', 'Já existe uma iniciativa do Proinfância cadastrada. Ao continuar, ela será desativada', 'warning');
                    }
                }
            );
        }
    });

    function exibirModalConfirmacaoInclusao(evt, avancarAba){
        //console.log(evt);
        evt.preventDefault();
        var iniid = $('#formIniciativa').find('[name=iniid]').val();
        var msg = 'Incluir';
        if (iniid) {
            msg = 'Alterar';
        }
        swal({
                title: "Confirmar",
                text: "Deseja <b>" + msg + "</b> esta Iniciativa",
                type: "success",
                html: true,
                showCancelButton: true,
                confirmButtonColor: "#DD6B55",
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    salvarIniciativa(avancarAba, false);
                }
                return false;
            }
        );
    }

    //Seleção do Ciclo
    $(document).on('change click', "#cicid", function () {
        var ciclo_id = $("#cicid").val();
        //console.log(ciclo_id);
        var caminho = window.location.href;
        var options = $("#iniano");
        var action = '&req=listarAnos&cicid=' + ciclo_id;
        $.getJSON(caminho + action, function (dados) {
            //console.log(dados);
            options.empty();
            if (!dados) {
                options.append(new Option("", ""));
            } else {
                //options.append(new Option('Selecione', '', 'selected'));
                $.each(dados, function (index, ano) {
                    options.append(new Option(ano.codigo, ano.descricao));
                });
            }
            options.focus();
            options.trigger('chosen:updated');
        });
    }).change();

    //Seleção da Área para filtrar os Programas
    $(document).on('change click', "#prgid", function () {
        var prgid = $("#prgid").val();
        //console.log(area_id);
        var caminho = window.location.href;
        var options = $("#iarid");
        var action = '&req=listarAreasProgramas&prgid=' + prgid;
        //alert(action);
        $.getJSON(caminho + action, function (dados) {
            //console.log('dados: ' +dados);
            options.empty();
            if (!dados) {
                options.append(new Option("", ""));
            } else {
                //options.append(new Option('Selecione', '', 'selected'));

                $.each(dados, function (index, programa) {
                    options.append(new Option(programa.descricao, programa.codigo));
                });
            }
            options.focus();
            options.trigger('chosen:updated');
        });
    }).change();

    //Seleção do Estado para filtrar os Municípios
    $(document).on('change click', "#estuf", function () {
        var estado_id = $("#estuf").val();
        var caminho = window.location.href;
        var options = $("#muncod");
        var action = '&req=listarMunicipios&estuf=' + estado_id;
        //console.log(caminho + action);
        $.getJSON(caminho + action, function (dados) {
            //alert(dados);
            //console.log('dados: ' +dados);
            options.empty();
            if (!dados) {
                options.append(new Option("", ""));
            } else {
                $.each(dados, function (index, municipio) {
                    options.append(new Option(municipio.descricao, municipio.codigo));
                });
            }
            options.focus();
            options.trigger('chosen:updated');
        });
    }).change();

    function atualizarEsfera(){
        if( $("#iniesfera").val() == 'E'){
            // Esconde o campo de Grupo de Municípios
            $('#estuf').append(new Option('Distrito Federal','DF'))
            $('#estuf').trigger('chosen:updated');
            $("#form-group-grupo-tipo-municipio").hide();
            $('#muncod').val('').trigger("chosen:updated");
            // Esconde o campo de Município
            $("#form-group-municipio").hide();
            $('#gtmid').val('').trigger("chosen:updated");
        }else{
            $("#estuf option[value='DF']").remove();
            $('#estuf').trigger('chosen:updated');
            $("#form-group-grupo-tipo-municipio").show();
            $("#form-group-municipio").show();
        }
    }

    //Seleção da Esfera para ocultar o campo Município caso escolha a opção Estadual.
    $(document).on('change click', "#iniesfera", function () {
        atualizarEsfera();
    });

    jQuery('#cancelarIniciativa').click(function (evt) {
        evt.preventDefault();
        window.location.href = "par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=listaIniciativas";
    });

    jQuery('#salvarIniciativa').click(function (evt) {
        exibirModalConfirmacaoInclusao(evt, false);
    });

    jQuery('#salvarSeguirProximo').click(function (evt) {
        salvarIniciativa(true, false);
    });

    function informarErros(res, avancarAba){
        $.each(res, function (index, erro) {//retorna mensagens de erro em json
            $.each(erro, function (err, errMsg) {
                // Edição: Caso tenha retornado o erro de que já existe uma iniciativa inativa igual,
                // exibe o modal perguntando para o usuário se deseja restaurar.
                if(err == "iniciativaInativaExistente"){
                    swal({
                            title: "Restaurar Iniciativa",
                            text: "Existe uma iniciativa descartada com os campos informados. Deseja restaurá-la?",
                            type: "success",
                            html: true,
                            showCancelButton: true,
                            confirmButtonColor: "#DD6B55",
                            cancelButtonText: "Cancelar",
                            confirmButtonText: "Sim",
                            closeOnConfirm: false
                        }, function (isConfirm) {
                            if (isConfirm) {
                                //swal.close();
                                salvarIniciativa(avancarAba, true);
                            } else {
                                // Se o usuário não quis restaurar, recarrega a página para que os campos voltem como estavam antes.
                                swal.close();
                                location.reload();
                            }
                            return false;
                        }
                    );
                } else {
                    var divFormInput = $("#formIniciativa").find("#" + err).parent("div");

                    if (errMsg < err.length) {
                        divFormInput.closest('.form-group').removeClass('has-error');
                        $("#div-" + err).remove();
                        return;//se a mensagem for vazia não retorna nada
                    } else{
                        divFormInput.closest('.form-group').addClass('has-error');
                        divFormInput.append("<div id='div-" + err + "'></div>");
                        $("#div-" + err).html("<span class='danger'>" + errMsg + "</span>");
                    }
                    //Verifica se exibe mensagem sobre já existência da iniciativa.
                    //console.log('err: '); console.log(err);
                    if(err == "validacaoExistencia" && errMsg.length > 0){
                        alert(errMsg);
                    }

                    swal.close();
                }
            });
        });
    }

    function solicitarInclusao(){

    }

    function salvarIniciativa(avancarAba, desconsiderarId) {
        // Ex: utilizado quando o usuário edita uma iniciativa em que já exista uma igual inativa...
        // é desconsiderado o id para que trabalhe da mesma forma que o insert.
        if(desconsiderarId == true){
            restaurarInativo = "restaurarInativo=1";
        } else {
            restaurarInativo = "";
        }
        var caminho = window.location.href;
        var action = '&guia=cadastroIniciativas&req=salvar&' + $("#formIniciativa").serialize() + '&' + restaurarInativo;
        //alert($("#formIniciativa").serialize());
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                //alert(resp);
                console.log('erros:'); console.log(resp);
                var res = $.parseJSON(resp);

                if(res.erro != '' && res.erro != undefined){
                    console.log(resp.erro);
                    swal({title: "Erro ao salvar",
                        text: res.erro,
                        type: 'error',
                        html: true
                    });

                    return false;
                }

                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    if(avancarAba){
                        window.location.href = "par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=cadastroIniciativaDetalhe&iniid=" + resp.replace(/['"]+/g, '');
                    } else {
                        caminho = "par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=cadastroIniciativas&iniid=" + resp.replace(/['"]+/g, '');
                        msgSuccess(caminho,"Iniciativa Salva com sucesso");
                        //
                    }

                    return false;
                }
                //swal.close();
                //console.log("res: "); console.log(res);
                informarErros(res, avancarAba);

                return false;
            }
        });
        return false;
    }

    atualizarEsfera();
</script>