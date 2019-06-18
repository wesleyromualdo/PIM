<?php

class Par3_Controller_Aviso_conteudo extends Controle
{
    private $model;

    public function __construct()
    {
        include_once APPRAIZ . 'par3/classes/model/Aviso_conteudo.class.inc';
//         $actid = validarInteiro($_REQUEST['actid']) ? $_REQUEST['actid'] : '';
        $actid = (is_numeric($_REQUEST['actid']) ? $_REQUEST['actid'] : '');
        $this->model = new Par3_Model_Aviso_conteudo($actid);

        if ($_REQUEST['requisicao'] == 'setarSessaoAvisoVisto') {
            ob_clean();
            if ($_REQUEST['actid']) {
                $_SESSION[$_SESSION['sisdiretorio']]['avisovisto'][] = $_REQUEST['actid'];
            }
            $_SESSION[$_SESSION['sisdiretorio']]['avisovisto'] = array_unique($_SESSION[$_SESSION['sisdiretorio']]['avisovisto']);
            die;
        } elseif ($_REQUEST['requisicao'] == 'setarCienteAvisoVisto') {
            ob_clean();
            if ($this->model->actid) {
                $arDado = array();
                $arDado['arChavePrimaria'] = 'acuid';
                $arDado['stNomeTabela'] = 'par3.aviso_conteudo_usuario';
                $arDado['arAtributosTabela'] = array('acuid', 'usucpf', 'actid', 'acuciente');

                $arDado['acuid'] = $arrPost['acuid'];
                $arDado['actid'] = $this->model->actid;
                $arDado['usucpf'] = $_SESSION['usucpf'];
                $arDado['acuciente'] = 't';

                $acuid = (int)trata_salvar($arDado);
            }
            die;
        }
    }

    public function salvarAvisoConteudo($arrPost)
    {
        if (strpos($arrPost["actconteudo"], '<p style=\"page-break-before: always;\"><!-- pagebreak --></p>')) {
            $arrPost["actconteudo"] = str_replace('<p style=\"page-break-before: always;\"><!-- pagebreak --></p>', '<p style="page-break-before:always"><!-- pagebreak --></p>', $arrPost["actconteudo"]);
        } else {
            $arrPost["actconteudo"] = str_replace("<!-- pagebreak -->", '<p style="page-break-before:always"><!-- pagebreak --></p>', $arrPost["actconteudo"]);
        }
        $arrPost["actconteudo"] = substituir_char_especiais_word($arrPost["actconteudo"]);
        $arrPost["actconteudo"] = str_replace('\"', '"', $arrPost["actconteudo"]);
        $arrPost["actconteudo"] = str_replace("\'", "'", $arrPost["actconteudo"]);

        $arDado = [];
        $arDado['arChavePrimaria'] = 'actid';
        $arDado['stNomeTabela'] = 'par3.aviso_conteudo';
        $arDado['arAtributosTabela'] = ['actid', 'usucpf', 'acttitulo', 'actconteudo', 'actpermiteciente', 'actinicio', 'actfim', 'acttipoesfera'];

        $arDado['actid'] = $arrPost['actid'];
        $arDado['usucpf'] = $_SESSION['usucpf'];
        $arDado['acttitulo'] = $arrPost['acttitulo'];
        $arDado['actconteudo'] = $arrPost['actconteudo'];
        $arDado['actpermiteciente'] = $arrPost['actpermiteciente'];
        $arDado['actinicio'] = $arrPost['actinicio'];
        $arDado['actfim'] = $arrPost['actfim'];
        $arDado['acttipoesfera'] = $arrPost['acttipoesfera'];

        $actid = (int) trata_salvar($arDado);

        if (!is_array($arrPost['estuf'])) {
            $arrPost['estuf'] = [$arrPost['estuf']];
        }

        if (!is_array($arrPost['muncod'])) {
            $arrPost['muncod'] = [$arrPost['muncod']];
        }

        if ($actid && $arrPost['pflcod']) {

            $sql = "update par3.aviso_conteudo_perfil set acpstatus = 'I' where actid = {$actid};";
            $this->model->executar($sql);

            $arDado = array();
            $arDado['arChavePrimaria'] = 'acpid';
            $arDado['stNomeTabela'] = 'par3.aviso_conteudo_perfil';
            $arDado['arAtributosTabela'] = array('actid', 'pflcod');

            $arDado['actid'] = $actid;
            foreach ($arrPost['pflcod'] as $pflcod) {
                $arDado['pflcod'] = $pflcod;
                $id = (int)trata_salvar($arDado);
            }
        }

        $sql  = "update par3.aviso_conteudo_estado set acestatus = 'I' where actid = {$actid}; ";
        $sql .= "update par3.aviso_conteudo_municipio set acmstatus = 'I' where actid = {$actid};";
        $this->model->executar($sql);
        $this->model->commit();
        
        if ($actid && $arrPost['estuf']) {
            $arDado = [];
            $arDado['arChavePrimaria'] = 'aceid';
            $arDado['stNomeTabela'] = 'par3.aviso_conteudo_estado';
            $arDado['arAtributosTabela'] = ['actid', 'estuf'];

            $arDado['actid'] = $actid;
            foreach ($arrPost['estuf'] as $estuf) {
                if (empty($estuf)) continue;
                $arDado['estuf'] = $estuf;
                $id = (int) trata_salvar($arDado);
            }
        }
        
        if ($actid && $arrPost['muncod']) {
            $arDado = [];
            $arDado['arChavePrimaria'] = 'acmid';
            $arDado['stNomeTabela'] = 'par3.aviso_conteudo_municipio';
            $arDado['arAtributosTabela'] = ['actid', 'muncod'];

            $arDado['actid'] = $actid;
            foreach ($arrPost['muncod'] as $muncod) {
                if (empty($muncod)) continue;
                $arDado['muncod'] = $muncod;
                $id = (int) trata_salvar($arDado);
            }
        }

        $url = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=aviso';
        if ($id > 0) {
            simec_redirecionar($url, 'success', 'Aviso cadastrado com sucesso!');
        } else {
            simec_redirecionar($url, 'error', 'Erro ao criar o aviso, tente novamente!');
        }
    }

    public function excluirAvisoConteudo()
    {
        $url = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=aviso';

        if ($this->model->actid) {
            $this->model->actstatus = 'I';
            $this->model->salvar();
            $this->model->commit();

            simec_redirecionar($url, 'success', 'Aviso excluído com sucesso!');
        }
        simec_redirecionar($url, 'error', 'Não foi possível excluir o aviso, tente novamente!');
    }

    public function carregarAvisoConteudo()
    {
        $arDado = $this->model->getDados();

        if ($arDado['actid']) {
            $sql = "select pflcod from par3.aviso_conteudo_perfil where actid = {$arDado['actid']} and acpstatus = 'A';";
            $arDado['pflcod'] = $this->model->carregarColuna($sql);
            $arDado['pflcod'] = ($arDado['pflcod'] ? $arDado['pflcod'] : array());


            $sql = "select muncod from par3.aviso_conteudo_municipio where actid = {$arDado['actid']} and acmstatus = 'A';";
            $arDado['muncod'] = $this->model->carregarColuna($sql);
            $arDado['muncod'] = ($arDado['muncod'] ? $arDado['muncod'] : array());

            $sql = "select estuf from par3.aviso_conteudo_estado where actid = {$arDado['actid']} and acestatus = 'A';";
            $arDado['estuf'] = $this->model->carregarColuna($sql);
            $arDado['estuf'] = ($arDado['estuf'] ? $arDado['estuf'] : array());
        }

        return $arDado;
    }

    public function listarAvisoConteudo($arrPost)
    {
        $sql = $this->model->sqlListar($arrPost);

        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Título', 'Período do aviso', 'Inserido por:', 'Perfis associados', 'Esfera', 'UF', 'Município', 'Data de criação');
        $esconderColunas = array('');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->setQuery($sql);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->addAcao('remove', array('func' => 'excluirAviso'));
        $listagemSimec->addAcao('edit', array('func' => 'editarAviso'));
        $listagemSimec->addAcao('view', array('func' => 'visualizarAviso'));
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setCampos($arrayCabecalho);
        $listagemSimec->setFormOff();
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }

    public function lancarAviso()
    {
        $_SESSION[$_SESSION['sisdiretorio']]['avisovisto'] = ($_SESSION[$_SESSION['sisdiretorio']]['avisovisto'] ? $_SESSION[$_SESSION['sisdiretorio']]['avisovisto'] : array());
        
        $arAviso = $this->model->carregarAvisosPorUsuario(array('not(actid)' => $_SESSION[$_SESSION['sisdiretorio']]['avisovisto']));
        if (count($arAviso) > 0) {
            $this->modalAvisoConteudo(array('actid' => $arAviso[0]['actid'], 'actconteudo' => $arAviso[0]['actconteudo']), $arAviso[0]['actpermiteciente']);
            array_shift($arAviso);
            $this->jsAvisoConteudo($arAviso);
        }
    }

    public function pegarConteudoAviso()
    {
        return $this->model->actconteudo;
    }

    public function modalAvisoConteudo($comConteudo, $actpermiteciente = false)
    {
        if ($comConteudo === true) {
            $actid = $this->modal->actid;
            $conteudoAviso = $this->modal->actconteudo;
        } elseif (is_array($comConteudo)) {
            $actid = $comConteudo['actid'];
            $conteudoAviso = $comConteudo['actconteudo'];
        }
        
        if ($_SESSION['sislayoutbootstrap'] == 'zimec') {
            include APPRAIZ . 'par3/modulos/sistema/tabelaapoio/aviso/modalAvisoBootstrap.inc';
        } else {
            include APPRAIZ . 'par3/modulos/sistema/tabelaapoio/aviso/modalAviso.inc';
        }
    }

    public function jsAvisoConteudo($arAviso)
    {
        if ($_SESSION['sislayoutbootstrap'] == 'zimec') {
            include_once APPRAIZ . 'par3/modulos/sistema/tabelaapoio/aviso/jsAvisoBootstrap.inc';
        } else {
            include_once APPRAIZ . 'par3/modulos/sistema/tabelaapoio/aviso/jsAviso.inc';
        }
    }

    public function carregaMunicipiosAviso($estuf)
    {
        global $db;
        $dados = $db->carregar(Par3_Model_Aviso_conteudo::getEstadoMunicipio($estuf));
        return simec_json_encode($dados);
    }
}