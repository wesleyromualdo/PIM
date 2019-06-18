<?php
/**
 * Implementação da classe Simec_ExtratoDinamico.
 *
 * @version $Id: ExtratoDinamico.php 143900 2018-09-14 13:56:00Z victormachado $
 */

require_once APPRAIZ . 'includes/library/simec/Listagem.php';
require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';
require_once APPRAIZ . 'public/classes/model/Extratodinamicoconsultas.class.inc';
require_once dirname(__FILE__) . '/extratodinamico/Form.php';
require_once dirname(__FILE__) . '/extratodinamico/Config.php';
require_once dirname(__FILE__) . '/extratodinamico/Relatorio.php';

/**
 *
 */
class Simec_ExtratoDinamico
{
    const QUALITATIVO = 'ql';
    const QUANTITATIVO = 'qt';
    const TOTALIZADO = 'tt';
    const FILTRO = 'fl';
    const CALLBACK = 'cb';
    const ID_DEFAULT_FORM = 'extrato';
    const FILTRO_QL = 'fql';
    const FILTRO_QT = 'fqt';

    protected $config;
    /**
     * @var Simec_ExtratoDinamico_Form Form e resultado da consulta do extrato.
     */
    protected $form;

    protected $erro;

    /**
     *
     * @param string $modulo Módulo do relatório - preferencialmente utilize $_SESSION['sisdiretorio'].
     * @param string $nome Identificador do relatório.
     */
    public function __construct($modulo, $nome = self::ID_DEFAULT_FORM)
    {
        try {
            $this->config = new Simec_ExtratoDinamico_Config($modulo, $nome);
            $this->form = new Simec_ExtratoDinamico_Form($this->getConfig());
        } catch (Exception $e) {
            $this->erro = $e->getMessage();
        }
    }

    public function getConfig()
    {
        return $this->config;
    }

    public function getForm()
    {
        return $this->form;
    }

    public function escondeCampoQualitativo() {
        $this->form->escondeCampoQualitativo();
        return $this;
    }

    public function escondeCampoQuantitativo() {
        $this->form->escondeCampoQuantitativo();
        return $this;
    }

    public function render()
    {
        if ($this->erro) {
            echo <<<HTML
<div class="col-lg-6 col-lg-offset-3 text-center">
    <div class="alert alert-danger" role="alert">{$this->erro}</div>
</div>
HTML;
            return;
        }

        echo $this->renderConsultasArmazenadas(),
            $this->getForm()->render(),
            (new Simec_ExtratoDinamico_Relatorio($this->getConfig(), $this->getForm()))
                ->render();
    }

    public function checaRequisicao(Simec_Helper_FlashMessage $fm)
    {
        if (!$this->getConfig()) {
            return $this;
        }

        if ($this->getForm()->salvarConsulta()) {
            $edConsulta = new Public_Model_Extratodinamicoconsultas();
            $edConsulta->popularDadosObjeto(
                $this->getForm()->getDadosPersistencia()
            );

            if ($edConsulta->salvar()) {
                $edConsulta->commit();

                $fm->addMensagem('Extrato "' . $edConsulta->edcnome . '" salvo com sucesso!');
            } else {
                $fm->addMensagem('Não foi possível salvar o extrato.', Simec_Helper_FlashMessage::ERRO);
            }
            header("Location: {$_SERVER['REQUEST_URI']}");
            die();
        }

        if ($this->getForm()->retornarConfiguracao()) {
            echo (new Public_Model_Extratodinamicoconsultas($this->getForm()->getIdConfiguracao()))->edcconfig;
            die();
        }

        return $this;
    }

    protected function renderConsultasArmazenadas()
    {
        $campos = 'edcid, edcnome, usunome, dataultimaalteracao, edcpublico';
        $clausulas = [
            "t1.exdid='{$this->getConfig()->getDbId()}'",
            "(t1.edcpublico OR t1.usucpf = '{$_SESSION['usucpf']}')"
        ];
        $ordem = 'edcnome';
        $opcoes = [
            'query' => true,
            'join' => ['usucpf', 'exdid']
        ];

        $relatorio = (new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO))
            ->turnOffForm()
            ->setQuery((new Public_Model_Extratodinamicoconsultas())->recuperarTodos($campos, $clausulas, $ordem, $opcoes))
            ->addCallbackDeCampo('edcpublico', 'Simec_ExtratoDinamico::formatarPublicidade')->addAcao('run', 'executarExtratoArmazenado')
//            ->addAcao('view', ['func' => 'visualizarExtratoArmazenado', 'titulo' => 'Visualizar'])
//            ->addAcao('delete', 'deletarExtratoArmazenado')
            ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        return <<<HTML
<div class="panel panel-info" id="painel_{$this->idForm}">
    <div class="panel-heading">
        <a role="button" data-toggle="collapse" href="#lista-consultas" aria-expanded="true"
           aria-controls="lista-consultas">Consultas armazenadas</a>
    </div>
    <div class="collapse" id="lista-consultas">{$relatorio}</div>
</div>
<script type="text/javascript">
function executarExtratoArmazenado(edcid)
{
    $.get(window.location.href, {'{$this->getForm()->getId()}[retornarConfiguracao]':edcid}, function(data){
        AgrupadorOrigemParaDestino();
        
        $('#{$this->getForm()->getId()} option:selected').removeAttr('selected');
        for (var item in data) {
            $('[name="{$this->getForm()->getId()}[' + item + '][]"]').val(data[item]).trigger('chosen:updated');
        }
        AgrupadorDestinoParaOrigem();
        
        $('#{$this->getForm()->getId()}').submit();
    }, 'json');
}
</script>
HTML;
    }

    /**
     * Formata o valor do campo edcpublic sem "Público" ou "Privado".
     *
     * @param string $edcpublico Indicador de publicidade do relatório
     * @return string
     */
    public static function formatarPublicidade($edcpublico)
    {
        if ('t' == $edcpublico) {
            return '<span class="label label-success">Público</span>';
        }

        if ('f' == $edcpublico) {
            return '<span class="label label-danger">Privado</span>';
        }
    }
}
