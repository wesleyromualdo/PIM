<?php
/**
 * Classe de controle da iniciativa
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 16-11-2016
 * @link     no link
 */

/**
 * Par3_Controller_EquipeLocal
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 16-11-2016
 * @link     no link
 */
class Par3_Controller_Iniciativa extends Controle
{
    private $model;

    public function __construct()
    {
        $iniid = validarInteiro($_REQUEST['iniid']) ? $_REQUEST['iniid'] : '';
        $this->model = new Par3_Model_Iniciativa($iniid);
        $this->modelHistorico = new Par3_Model_IniciativaHistorico();
    }

    public function salvar($arrPost)
    {
        $acao = Par3_Model_IniciativaHistorico::CREATE;
        if ($arrPost['iniid']) {
            $arrayIniciativa['iniid'] = $arrPost['iniid'];
            $acao = Par3_Model_IniciativaHistorico::UPDATE;
        }

        $opcoesValidacaoInclusao = array(
            'desconsiderarCamposPreenchimento' => array()
        );

        $arrayIniciativa['inistatus'] = 'A';
        $arrayIniciativa['inisituacao'] = $arrPost['inisituacao'];
        $arrayIniciativa['indid'] = $arrPost['indid'];
        $arrayIniciativa['intaid'] = $arrPost['intaid'];
        $arrayIniciativa['cicid'] = $arrPost['cicid'];
        $arrayIniciativa['intoid'] = $arrPost['intoid'];
        $arrayIniciativa['iniesfera'] = $arrPost['iniesfera'];
        $arrayIniciativa['inivalidapendencia'] = $arrPost['inivalidapendencia'];
        $arrayIniciativa['initodos_estuf'] = is_array($arrPost['estuf']) ? (array_search('T', $arrPost['estuf']) === false ? "N" : "S") : "N";
        $arrayIniciativa['initodos_muncod'] = is_array($arrPost['muncod']) ? (array_search('T', $arrPost['muncod']) === false ? "N" : "S") : "N";
        $arrayIniciativa['iniobraproinfancia'] = $arrPost['iniobraproinfancia'];
        // Cria array que terá os campos das tabelas externas.
        $arrayIniciativaTbExterna = $arrayIniciativa;
        $arrayIniciativaTbExterna['iarid'] = $arrPost['iarid'] ? $arrPost['iarid'] : array();
        $arrayIniciativaTbExterna['proid'] = $arrPost['proid'] ? $arrPost['proid'] : array();
        $arrayIniciativaTbExterna['prgid'] = $arrPost['prgid'] ? $arrPost['prgid'] : array();
        // Se existir o valor T (Todos) no estado, considera como nenhum valor, pois assim removerá (inativará) os que já existem,
        // mas insere opção na validação para não verificar o seu preehcimento.
        $arrayIniciativaTbExterna['estuf'] = array();
        if ($arrPost['estuf']) {
            if (array_search('T', $arrPost['estuf']) === false) {
                $arrayIniciativaTbExterna['estuf'] = $arrPost['estuf'];
            } else {
                $arrayIniciativaTbExterna['estuf'] = array();
                array_push($opcoesValidacaoInclusao['desconsiderarCamposPreenchimento'], 'estuf');
            }
        }
        $arrayIniciativaTbExterna['estuf'] = $arrPost['estuf'] ? (array_search('T', $arrPost['estuf']) === false ? $arrPost['estuf'] : array()) : array();
        $arrayIniciativaTbExterna['gtmid'] = $arrPost['gtmid'] ? array_filter($arrPost['gtmid']) : array();
        $arrayIniciativaTbExterna['iniano'] = $arrPost['iniano'] ? $arrPost['iniano'] : array();
        // Se existir o valor T (Todos) no município, considera como nenhum valor, pois assim removerá (inativará) os que já existem,
        // mas insere opção na validação para não verificar o seu preehcimento.
        $arrayIniciativaTbExterna['muncod'] = array();
        if ($arrPost['muncod']) {
            if (array_search('T', $arrPost['muncod']) === false) {
                $arrayIniciativaTbExterna['muncod'] = array_filter($arrPost['muncod']);
            } else {
                $arrayIniciativaTbExterna['muncod'] = array();
                array_push($opcoesValidacaoInclusao['desconsiderarCamposPreenchimento'], 'muncod');
            }
        }
        // Deixar os valores do campo de município entre áspas simples.
        $municipios = array();
        foreach ($arrayIniciativaTbExterna['muncod'] as $municipio) {
            array_push($municipios, "'$municipio'");
        }
        $arrayIniciativaTbExterna['muncod'] = $municipios;

        //Valida Inclusão
        $opcoesValidacaoInclusao['tipo'] = $arrPost['iniid'] ? 'update' : 'insert';
        $arrayIniciativaValidacao = $arrayIniciativaTbExterna;
        $arrayIniciativaValidacao['iniid'] = $arrPost['iniid'] ? $arrPost['iniid'] : null;
        $arrayIniciativaValidacao['indid'] = $arrPost['indid'] ? $arrPost['indid'] : null;
        $erro = $this->model->validaInclusao($arrayIniciativaValidacao, $opcoesValidacaoInclusao);
        if (array_filter($erro['erros'])) {
            return $erro;
        }

        if($arrayIniciativa['iniobraproinfancia'] == 'true'){
            $idinativacao = $this->verificaIniciativaProinfancia();

            if($idinativacao > 0){
                $result = $this->inativar(['iniid' => $idinativacao]);
                if($result != 'erro' && !is_numeric($result)){
                    return ['erro' => 'A iniciativa de código <b>' . $idinativacao . '</b> não pode ser inativada pois já está relacionada a um município.'];
                }
            }
        }

        //Verifica restauração
        //Se for criação, irá ativar a iniciativa que tiver todos os campos iguais em vez de criar uma nova. Se não existir com campos iguais, cria.
        //Se for edição e existir uma iniciativa igual inativa, irá perguntar ao usuário se deseja restaurá-la. Se escolher "Sim", restaura, se "Não" não ocorre alteração.
        $iniciativaInativada = $this->model->verificarRestauracao($arrayIniciativaTbExterna);
        //print_r($arrPost); exit();
        try {
            // Desativada as funcionalidades de verificação/ação na criação e edição sobre iniciativas iguais que já foram excluídas (inativada)
            /*
            // Se for insert e existir a iniciativa inativada igual, ativa, grava o histórico e retorna o seu id.
            if( (
                empty($arrPost['iniid']) && $iniciativaInativada)
                // Ou se for edição e for para restaurar uma iniciativa inativa:
                || (!empty($arrPost['iniid']) && array_key_exists('restaurarInativo', $arrPost) && $arrPost['restaurarInativo'] == 1)
            ){
                // Ativa a iniciativa.
                $arrayIniciativaInativada = $arrayIniciativa;
                $arrayIniciativaInativada['iniid'] = $iniciativaInativada[0]['iniid'];
                $arrayIniciativaInativada['inistatus'] = 'A';
                $this->model->popularDadosObjeto($arrayIniciativaInativada);
                //print_r($arrayIniciativaInativada); exit();
                $id =  $this->model->salvar();
                $retorno = $id;
                $this->model->commit();
                // Grava o histórico
                $modelIniciativaInativada = new Par3_Model_Iniciativa($id);
                $this->modelHistorico->gravarHistorico($modelIniciativaInativada,$acao);
                $this->modelHistorico->commit();
            // Se for update e existir uma iniciativa inativada igual, irá mandar mensagem para o usuário para saber se deseja restaurá-la.
            } else if($arrPost['iniid'] && $iniciativaInativada){
                $retorno = array('erros'=>array('iniciativaInativaExistente'=>array('iniid'=>$iniciativaInativada[0]['iniid'])));
                //$retorno = array('teste'=>array());
            } else {*/

            $this->model->popularDadosObjeto($arrayIniciativa);
            $id = $this->model->salvar();
            $retorno = $id;
            $this->model->commit();

            //Salvar Anos
            $controllerIniciativaAnos = new Par3_Controller_IniciativaIniciativasAnos();
            $idIniciativaAno = $controllerIniciativaAnos->salvar(array('iniid' => $id, 'iniano' => $arrayIniciativaTbExterna['iniano']));
            //Salvar Área Relacionada na iniciativa
            $controllerIniciativaArea = new Par3_Controller_IniciativaIniciativasAreas();
            $idIniciativaArea = $controllerIniciativaArea->salvar(array('iniid' => $id, 'iarid' => $arrayIniciativaTbExterna['iarid']));
            //Salvar Programas na iniciativa
            $controllerIniciativaProgramas = new Par3_Controller_IniciativaIniciativasProgramas();
            $idIniciativaProgramas = $controllerIniciativaProgramas->salvar(array('iniid' => $id, 'prgid' => $arrayIniciativaTbExterna['prgid']));
            //Salvar Projetos na iniciativa
            $controllerIniciativaProjetos = new Par3_Controller_IniciativaIniciativasProjetos();
            $idIniciativaProjetos = $controllerIniciativaProjetos->salvar(array('iniid' => $id, 'proid' => $arrayIniciativaTbExterna['proid']));
            //Salvar Estados na iniciativa
            $controllerIniciativaEstados = new Par3_Controller_IniciativaIniciativasEstados();
            $idIniciativaEstados = $controllerIniciativaEstados->salvar(array('iniid' => $id, 'estuf' => $arrayIniciativaTbExterna['estuf']));
            //Salvar Tipo Grupo de Municípios na iniciativa
            $controllerIniciativaGrupoTipoMunicipio = new Par3_Controller_IniciativaIniciativasGrupoTipoMunicipio();
            $idIniciativaGrupoTipoMunicipio = $controllerIniciativaGrupoTipoMunicipio->salvar(array('iniid' => $id, 'gtmid' => $arrayIniciativaTbExterna['gtmid']));
            //Salvar Tipo Municípios na iniciativa
            $controllerIniciativaMunicipios = new Par3_Controller_IniciativaIniciativasMunicipios();
            $idIniciativaMunicipios = $controllerIniciativaMunicipios->salvar(array('iniid' => $id, 'muncod' => $arrayIniciativaTbExterna['muncod']));
            //Registra o histórico
            $modelIniciativa = new Par3_Model_Iniciativa($id);
            $this->modelHistorico->gravarHistorico($modelIniciativa, $acao);
            $this->modelHistorico->commit();

        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($retorno) {
            return $retorno;
        }
        return 'erro';
    }

    public function inativar($arrPost)
    {
        $arrayIniciativa['iniid'] = $arrPost['iniid'];
        $arrayIniciativa['inistatus'] = "I";

        $mInp = new Par3_Model_IniciativaPlanejamento();
        $arrInp = $mInp->recuperarPorIniciativa($arrayIniciativa['iniid']);
        if (!empty($arrInp)) {
            return 'Esta Iniciativa está Relacionada a Um Município/Estado';
        }

        try {
            $this->model->popularDadosObjeto($arrayIniciativa);
            $sucesso = $this->model->salvar();
            $id = $this->model->commit();

            $modelIniciativa = new Par3_Model_Iniciativa($arrayIniciativa['iniid']);
            $this->modelHistorico->gravarHistorico($modelIniciativa, Par3_Model_IniciativaHistorico::DELETE);
            return $id;
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
    }

    public function recuperar($arrPost)
    {
        $sql = $this->model->montarSQLSimples($arrPost);
        return $this->model->carregar($sql);
    }

    public function listar($arrPost)
    {
        //print_r($arrPost); exit();
        $arrPost['iniid'] = $arrPost['iniid'];
        global $disabled;
        $modelIniciativa = new Par3_Model_Iniciativa();

        $campo = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC' ? 'ASC' : 'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;
        switch ($campo) {
            case 'codigo':
                $arrQuery['ordenacao']['campo'] = 'codigo';
                break;
            case 'etapas':
                $arrQuery['ordenacao']['campo'] = 'etapas';
                break;
            default:
                $arrQuery['ordenacao'] = '';
                break;
        }

        $sql = $modelIniciativa->montarSQLSimples($arrQuery);
        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        //Cabeçalho: código,descrição,situção
        $cabecalho = array('Código', 'Iniciativa', 'Tipo de Objeto', 'Tipo de Atendimento', 'Dimensão', 'Ciclo PAR', 'Área Relacionada', 'Programa', 'Situação');
        $esconderColunas = array('id', 'qtd');

        if ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir') {
            $cabecalho = array(
                'Código', 'Descrição da Iniciativa',
                'Tipo de Atendimento', 'Ciclo',
                'Ano', 'Tipo de Objeto',
                'Área Relacionada', 'Esfera',
                'Projeto', 'Programa',
                'Estado', 'Municipio',
                'Grupo de Municípios', 'Nível',
                'Etapa', 'Modalidade',
                'Desdobramento', 'Dimensão',
                'Área', 'Indicador',
                'Componentes do Indicador', 'Nome do Item de Composição',
                'Grupo de Itens de Composição', 'Situação'
            );
            $esconderColunas = array('id', 'cicid', 'iarid', 'proid', 'prgid', 'dimid', 'intoid', 'iniesfera');
            array_unshift($cabecalho, 'QTD');
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;
            $disabled = true;
        }

        $tipoRelatorio = ($arrPost['req'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;

        if ($arrPost['req'] == 'imprimir') {
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;
            $disabled = true;
        }//desabilitar opções para a impressão
        $tratamentoListaVazia = ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setId('iniciativa');
        // Se for para impressão, para caber todas as colunas na página, quebra as informações para exibir 8 - 10 para a primeira - colunas a cada 2 linhas (1ª é o cabeçalho e segunda os dados).
        if ($arrPost['req'] == 'imprimir') {
            $arrResultado = $modelIniciativa->carregar($sql);
            $arrImpressao = array();

            foreach ($arrResultado as $index => $r) {
                // Monta estrutura das primeiras 10 colunas:
                $cabecalho1 = array(
                    'QTD', 'Código', 'Descrição da Iniciativa', 'Tipo de Atendimento', 'Ciclo', 'Ano', 'Tipo de Objeto', 'Área Relacionada', 'Esfera', 'Projeto'
                );
                $dadosCabecalho1 = array(
                    $r['qtd'], $r['codigo'], $r['descricao'], $r['tipo_atendimento'],
                    $r['ciclo'], $r['ano'], $r['tipo_objeto'], $r['area_relacionada'],
                    $r['esfera'], $r['projeto']
                );
                // Monta estrutura das segundas 10 colunas:
                $cabecalho2 = array('', '', 'Programa', 'Estado', 'Município', 'Grupo de Muncípio', 'Nível', 'Etapa', 'Modalidade', 'Desdobramento');
                $dadosCabecalho2 = array('', '', $r['programa'], $r['estado'], $r['municipio'], $r['grupo_municipio'], $r['nivel'], $r['etapa'], $r['modalidade'], $r['desdobramento']);
                // Monta estrutura das terceiras 10 colunas:
                $cabecalho3 = array('', '', 'Dimensão', 'Área', 'Indicador', 'Componentes do Indicador', 'Nome do Item de Composição', 'Grupo de Itens de Composição', 'Situação');
                $dadosCabecalho3 = array('', '', $r['dimensao'], $r['area'], $r['indicador'], $r['componente'], $r['item_composicao'], $r['item_composicao_grupo'], $r['situacao']);
                // Insere as informações montadas acima no array dos dados para impressão ($arrImpressao)
                array_push($arrImpressao, $cabecalho1);
                array_push($arrImpressao, $dadosCabecalho1);
                array_push($arrImpressao, $cabecalho2);
                array_push($arrImpressao, $dadosCabecalho2);
                array_push($arrImpressao, $cabecalho3);
                array_push($arrImpressao, $dadosCabecalho3);
            }
            array_push($arrImpressao, array('Total de registros: ' . count($arrImpressao) / 6));
            $listagem->setDados($arrImpressao);
            // Não é impressão então não necessita de reestruturação
        } else {
            $listagem->esconderColunas($esconderColunas);
            $listagem->setCabecalho($cabecalho);
            $listagem->setQuery($sql);
        }

        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        //$listagem->setTamanhoPagina(1);
        $listagem->addCallbackDeCampo('inisituacao', 'formata_status');
        if ($disabled == '') {
            $listagem->addAcao('remove', 'removerIniciativa');
            $listagem->addAcao('edit', 'editarIniciativa');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }

    /**
     *
     * @param $opcao
     * @return string
     */
    public function controlarRequisicaoJson($opcao)
    {

        switch ($opcao) {

            //Lista de etapas
            case "etapas":

                ob_clean();
                $nivid = $_GET['nivid'];
                //instanciamento da classe Par3_Model_EnsinoEtapa
                $objEnsinoEtapa = new Par3_Model_EnsinoEtapa();
                $retorno = $objEnsinoEtapa->listaEnsinoEtapas($nivid);

                break;

            //Lista de modalidades
            case "modalidades":

                ob_clean();
                $etaid = $_GET['etaid'];
                //instanciamento da classe Par3_Model_Modalidade
                $objModalidade = new Par3_Model_Modalidade();
                $retorno = $objModalidade->listaModalidade($etaid);

                break;

            //Lista de áreas
            case "areas":

                ob_clean();
                $dimid = $_GET['dimid'];
                //instanciamento da classe Par3_Model_Area
                $objArea = new Par3_Model_Area();
                $retorno = $objArea->listaAreas($dimid);

                break;

            //Lista de indicadores
            case "indicadores":

                ob_clean();
                $areid = $_GET['areid'];
                //instanciamento da classe Par3_Model_Area
                $objIndicador = new Par3_Model_Indicador();
                $retorno = $objIndicador->listaIndicadores($areid);

                break;

            //Lista de Componentes do Indicador
            case "componentesIndicador":

                ob_clean();
                $indid = $_GET['indid'];
                //instanciamento da classe Par3_Model_Criterio
                $objCriterio = new Par3_Model_Criterio();
                $retorno = $objCriterio->listaComponentesIndicador($indid);

                break;
            default:
                $retorno = ['retorno' => 'Opção não encontrada ou inválida.'];
        }

        //Verifica se retornou falso.
        if ($retorno === false) {
            $retorno = [];
        }

        return simec_json_encode($retorno);

    }

    /* Segundo o Eduardo Dunice, o código abaixo se encontra legado. */

    /**
     * Função formSubacao
     * - monta o formulario de equipe local
     *
     * @return string escreve a lista.
     *
     */
    public function formSubacao($dados, $disabled = 'disabled')
    {
        global $simec;

        $disabled = '';

        $controlIniciativa = new Par3_Controller_Iniciativa();
        $modelIniciativa = new Par3_Model_Iniciativa();
        $modelMunicipio = new Par3_Model_Municipio();
        $modelUnidade = new Par3_Model_InstrumentoUnidade($dados['inuid']);
        $modelMunicipios = new Territorios_Model_Municipio();

        $arrOrcamentoUnidade = $modelUnidade->carregaOrcamentoUnidade($dados['inuid']);

        $arrDadosIniciativa = $modelIniciativa->recuperarIniciativa($dados);

        if (!$arrDadosIniciativa['sbaid']) {
            $arrDadosIniciativa['sbaid'] = $modelIniciativa->criaIniciativa($dados);
            $arrDadosIniciativa = $modelIniciativa->recuperarIniciativa($dados);
        }
        if (!$arrDadosIniciativa['sbdid']) {
            $arrDadosIniciativa['sbdid'] = $modelIniciativa->criaIniciativaDetalhe($arrDadosIniciativa);
        }

        $sqlMunicipiosUF = $modelMunicipio->montaSQLComboMunicipiosUF($dados['inuid']);

        include_once APPRAIZ . 'par3/modulos/principal/planoTrabalho/programas/iniciativa.php';
    }//end formSubacao()

    public function formItensPropostaIniciativa($dados, $disabled = 'disabled')
    {
        global $simec;

        $modelIniciativa = new Par3_Model_Iniciativa();
        $arrDadosItensPropostaIniciativa = $modelIniciativa->recuperarItensPospostaIniciativa($dados);

        include_once APPRAIZ . 'par3/modulos/principal/planoTrabalho/programas/iniciativa/itensPropostaIniciativa.php';
    }//end formItensPropostaIniciativa()

    /**
     *Função para gerar o DataGrid
     */
    public function listarIniciativa($arrPost)
    {

    }

    public function formItensIniciativa($dados, $disabled = 'disabled')
    {
        global $simec;

        $modelIniciativa = new Par3_Model_Iniciativa();
        $arrDadosItensIniciativa = $modelIniciativa->recuperarItensIniciativa($dados);

        include_once APPRAIZ . 'par3/modulos/principal/planoTrabalho/programas/iniciativa/itensIniciativa.php';
    }//end formItensIniciativa()

    public function formItensIniciativaEscola($dados, $disabled = 'disabled')
    {
        global $simec;

        $modelIniciativa = new Par3_Model_Iniciativa();
        $arrDadosItensIniciativa = $modelIniciativa->recuperarItensIniciativaEscola($dados);

        include_once APPRAIZ . 'par3/modulos/principal/planoTrabalho/programas/iniciativa/itensIniciativaEscola.php';
    }//end formItensIniciativaEscola()

    public function formQtdItensIniciativaEscola($dados, $disabled = 'disabled')
    {
        global $simec;

        $controlIniciativa = new Par3_Controller_Iniciativa();
        $modelIniciativa = new Par3_Model_Iniciativa();
        $arrDadosItensIniciativa = $modelIniciativa->recuperarQtdItensIniciativaEscola($dados);

        include APPRAIZ . 'par3/modulos/principal/planoTrabalho/programas/iniciativa/qtdItensIniciativaEscola.php';
    }//end formQtdItensIniciativaEscola()

    public function formEscolasMunicipio($dados, $disabled = 'disabled')
    {
        global $simec;

        $modelUnidade = new Par3_Model_InstrumentoUnidade($dados['inuid']);
        $modelIniciativa = new Par3_Model_Iniciativa();
        $arrEscolas = $modelIniciativa->recuperarEscolasMunicipio($modelUnidade, $dados);

        include_once APPRAIZ . 'par3/modulos/principal/planoTrabalho/programas/iniciativa/escolasMunicipio.php';
    }//end formEscolasMunicipio()

    public function listaEscolasIniciativa($dados, $disabled = 'disabled')
    {
        global $simec;

        $modelIniciativa = new Par3_Model_Iniciativa();
        $arrEscolas = $modelIniciativa->recuperarEscolasIniciativa($dados);

        include_once APPRAIZ . 'par3/modulos/principal/planoTrabalho/programas/iniciativa/escolasIniciativa.php';
    }//end listaEscolasIniciativa()

    public function vinculaItemComposicao($dados)
    {
        global $simec;

        $modelIniciativa = new Par3_Model_Iniciativa();
        $arrItem = $modelIniciativa->buscaItemComposicao($dados);

        switch ($arrItem['icostatus']) {
            case 'A':
                $modelIniciativa->inativaItemComposicao($arrItem['icoid']);
                break;
            case 'I':
                $modelIniciativa->ativaItemComposicao($arrItem['icoid']);
                break;
            case '':
                $modelIniciativa->insereItemComposicao($dados);
                break;
        }
    }

    public function vinculaEscolaSubacao($dados)
    {
        global $simec;

        require_once APPRAIZ . 'par/classes/modelo/SubacaoEscola.class.inc';

        $oSes = new SubacaoEscola();
        $dados['sesid'] = $oSes->recuperaEscolaPorSubacao($dados['sbaid'], $dados['ano'], $dados['entid']);

        if ($dados['sesid']) {
            $oSes = new SubacaoEscola($dados['sesid']);
            $oSes->sesstatus = $oSes->sesstatus == 'A' ? 'I' : 'A';
            $oSes->salvar();
            $oSes->commit();
        } else {
            $oSes->escid = $oSes->retornaEscolaPar($dados['entid']);
            $oSes->sesano = $dados['ano'];
            $oSes->sesstatus = "A";
            $oSes->sbaid = $dados['sbaid'];
            $oSes->salvar();
            $oSes->commit();
        }
    }//end vinculaItemComposicao()

    public function salvaIniciativa($dados)
    {
        global $simec;

        $dados['total_itens'] = str_replace(array('.', ','), array('', '.'), $dados['total_itens']);

        if ($dados['total_itens'] > $dados['valor_restante']) {
            echo 'O valor da iniciativa ultrapassa o valor disponível no orçamento.';
            die();
        }

        $modelIniciativa = new Par3_Model_Iniciativa();

        if (!empty($dados)) {
            $modelIniciativa->salvaIniciativaDetalhe($dados);
        }

        if ($dados['ppscronograma'] == '2') {
            if (is_array($dados['icoquantidade'])) {
                foreach ($dados['icoquantidade'] as $icoid => $qtdEscolas) {
                    foreach ($qtdEscolas as $sesid => $seiqtd) {
                        if ($seiqtd != '') {
                            $modelIniciativa->salvaQuantidadeItemComposicaoEscola($sesid, $icoid, $seiqtd);
                        }
                    }
                }
            }
        } else {
            if (is_array($dados['icoquantidade'])) {
                foreach ($dados['icoquantidade'] as $icoid => $icoquantidade) {
                    if ($icoquantidade != '') {
                        $modelIniciativa->salvaQuantidadeItemComposicao($icoid, $icoquantidade);
                    }
                }
            }
        }
        echo 'secesso';
    }//end salvaIniciativa()

    public function montaSelectPI($dados)
    {
        global $simec;

        $modelIniciativa = new Par3_Model_Iniciativa();
        $sql = $modelIniciativa->montaSQLPI($dados);

        echo $simec->select('sbdplanointerno', 'Plano Interno', $dados['sbdplanointerno'], $sql, array($disabled));
    }//end montaSelectPI()

    public function montaSelectPTRES($dados)
    {
        global $simec;

        $modelIniciativa = new Par3_Model_Iniciativa();
        $sql = $modelIniciativa->montaSQLPTRES($dados);

        echo $simec->select('sbdptres', 'PTRES', $dados['sbdptres'], $sql, array($disabled));
    }//end montaSelectPTRES()

    public function montaSelectPTRESPI($dados)
    {
        global $simec;

        $modelIniciativa = new Par3_Model_Iniciativa();
        $sql = $modelIniciativa->montaSQLPTRES($dados);
        $arrDados = $modelIniciativa->carregar($sql);
        $arrDados = is_array($arrDados) ? $arrDados : array();

        foreach ($arrDados as $dados) {
            echo "<option value={$dados['codigo']} >.{$dados['descricao']}.</option>";
        }
    }//end montaSelectPTRES()


    public function getIniciativaById($iniid)
    {
        return $this->model->getIniciativaById($iniid);
    }

    public function getIniciativaObraProinfancia(){
        return $this->model->getIniciativaProinfancia();
    }

    /**
     * @return string
     */
    function getSqlRelatorioGerencial()
    {
        return $this->model->getSqlRelatorioGerencial( $_REQUEST );
    }

    function getSqlTotalizadorRelatorioGerencial($request = [], $sqlPrincipal)
    {
        return $this->model->getSqlTotalizadorRelatorioGerencial( $request, $sqlPrincipal );
    }

    /**
     * Apenas para obras do programa PROINFANCIA
     * @return mixed
     */
    function getDadosSolicitacaoMobiliario($itrid){
        return $this->model->getDadosSolicitacaoMobiliario($itrid);
    }

    function verificaIniciativaProinfancia(){
        return $this->model->verificaIniciativaProinfancia();
    }

}//end class

?>