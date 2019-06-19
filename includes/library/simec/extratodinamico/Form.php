<?php
/**
 * Implementação da classe responsável por montar o formulário do extrato dinâmico.
 *
 * @version $Id: Form.php 143900 2018-09-14 13:56:00Z victormachado $
 */

require_once APPRAIZ . 'includes/library/simec/view/Form.php';
require_once APPRAIZ . 'includes/funcoesspo_componentes.php';
require_once dirname(__FILE__) . '/../ExtratoDinamico.php';
require_once dirname(__FILE__) . '/Config.php';

class Simec_ExtratoDinamico_Form
{
    const FORM_POSFIX = '_formarmazenar';
    const POPUP_POSFIX = '_popuparmazenar';

    protected $config;
    protected $dadosRequisicao;
    protected $dadosPersistencia = [];
    protected $dadosConsulta = [];

    public $mostracampoqualitativo = true;
    public $mostracampoquantitativo = true;

    public function __construct (Simec_ExtratoDinamico_Config $config)
    {
        $this->config = $config;
        $this->dadosRequisicao = $_POST[$this->getId()];

        if (isset($this->dadosRequisicao['salvarconsulta']))
        {
            $this->dadosPersistencia = $this->dadosRequisicao['salvarconsulta'];
            unset($this->dadosRequisicao['salvarconsulta']);
            $this->dadosPersistencia['edcconfig'] = simec_json_encode($this->dadosRequisicao);
            $this->dadosPersistencia['usucpf'] = $_SESSION['usucpf'];
            $this->dadosPersistencia['exdid'] = $this->getConfig()->getDbId();
        }

        if (isset($_GET[$this->getId()]['retornarConfiguracao']))
        {
            $this->dadosConsulta['edcid'] = $_GET[$this->getId()]['retornarConfiguracao'];
        }

        $this->normalizaDadosFormulario();
    }

    public function getId ()
    {
        return $this->getConfig()->getNome();
    }

    public function getDadosRequisicao ()
    {
        return $this->dadosRequisicao;
    }

    public function getDadosPersistencia ()
    {
        return $this->dadosPersistencia;
    }

    public function formularioSubmetido ()
    {
        return !empty($_POST);
    }

    public function salvarConsulta ()
    {
        return !empty($this->dadosPersistencia);
    }

    public function retornarConfiguracao ()
    {
        return !empty($this->dadosConsulta);
    }

    public function getIdConfiguracao ()
    {
        return $this->dadosConsulta['edcid'];
    }

    public function escondeCampoQualitativo() {
        $this->mostracampoqualitativo = false;
        return $this;
    }

    public function escondeCampoQuantitativo() {
        $this->mostracampoquantitativo = false;
        return $this;
    }

    public function render ()
    {
        $configSeparadorCol1 = ['tag' => 'legend', 'col' => Simec_View_Form::COLUNA_1];
        $configSeparadorCol2 = ['tag' => 'legend', 'col' => Simec_View_Form::COLUNA_2];

        if (!$this->mostracampoqualitativo && !$this->mostracampoquantitativo) {
            $configSeparadorCol1 = ['tag' => 'legend'];
            $configSeparadorCol2 = ['tag' => 'legend'];
        }

        $configCombosCol1 = ['col' => Simec_View_Form::COLUNA_1, 'multiple' => true];

        $QLdestino = [];
        if ($this->dadosRequisicao['ql'])
        {
            foreach ($this->dadosRequisicao['ql'] as $index => $item)
            {
                foreach ($this->getConfig()->getColunasQualitativas() as $optionQualitativo)
                {
                    if ($optionQualitativo['codigo'] == $item)
                    {
                        $QLdestino[] = $optionQualitativo;
                    }
                }
            }
        }

        $QTdestino = [];
        if ($this->dadosRequisicao['qt'])
        {
            foreach ($this->dadosRequisicao['qt'] as $index => $item)
            {
                foreach ($this->getConfig()->getColunasQuantitativas() as $optionQuantitativo)
                {
                    if ($optionQuantitativo['codigo'] == $item)
                    {
                        $QTdestino[] = $optionQuantitativo;
                    }
                }
            }
        }


        $form = new Simec_View_Form($this->getId());
        if ($this->mostracampoquantitativo || $this->mostracampoqualitativo) {
            $form->setLayout(Simec_View_Form::LAYOUT_2_COLUNAS);
        }

        if ($this->mostracampoqualitativo) {
            $form->carregarDados($this->dadosRequisicao)
                ->addSeparador('Colunas Qualitativas', $configSeparadorCol1)
                ->addAgrupador($this->getConfig()->getColunasQualitativas(), $QLdestino, 'qualcol1', 'ql');
        }

        if ($this->mostracampoqualitativo && $this->mostracampoquantitativo) {
            $form
                ->addSeparador('', ['tag' => 'br'])
                ->addSeparador('', ['tag' => 'br']);
        }

        if ($this->mostracampoquantitativo) {
            $form
                ->addSeparador('Colunas Quantitativas', $configSeparadorCol1)
                ->addAgrupador($this->getConfig()->getColunasQuantitativas(), $QTdestino, 'quantcol1', 'qt');
        }

        $form->addSeparador('Filtros', $configSeparadorCol2);

        $configComboFiltros = ['multiple' => true, 'col' => Simec_View_Form::COLUNA_2, 'formatolabel' => 'dual'];
        if (!$this->mostracampoqualitativo && !$this->mostracampoquantitativo) {
            $configComboFiltros = ['multiple' => true, 'formatolabel' => 'dual'];
        }
        foreach ($this->getConfig()->getFiltros() as $coluna => $filtro)
        {
            $form->addCombo($filtro['label'], $coluna, $filtro['query'], $configComboFiltros);
        }

        return $form->addBotoes(['limpar', 'buscar', 'salvar' => ['value' => 'salvar-consulta', 'role' => 'button', 'cor' => 'success']])
                ->render(false) . $this->renderJS() . $this->renderPopupSalvar();
    }

    /**
     * Retorna a lista de colunas selecionadas para o extrato.
     *
     * Se informado um tipo, apenas as colunas daquele tipo são retornadas, caso contrário,
     * é retornada uma lista completa contendo primeiro colunas qualitativas e, na sequencia,
     * as colunas quantitativas.
     *
     * @param int $tipo Tipo de colunas que devem ser retornadas.
     * @return string[]
     * @throws Exception
     */
    public function getColunasSelecionadas ($tipo = null)
    {
        $requisicaoQualitativo = $this->dadosRequisicao[Simec_ExtratoDinamico::QUALITATIVO];
        $requisicaoQuantitativo = $this->dadosRequisicao[Simec_ExtratoDinamico::QUANTITATIVO];

        if (!is_array($requisicaoQualitativo))
        {
            $requisicaoQualitativo = [];
        }

        if (!is_array($requisicaoQuantitativo))
        {
            $requisicaoQuantitativo = [];
        }

        if (empty($tipo))
        {
            return array_merge($requisicaoQualitativo, $requisicaoQuantitativo);
        }

        if (Simec_ExtratoDinamico::QUALITATIVO == $tipo)
        {
            return $requisicaoQualitativo;
        }

        if (Simec_ExtratoDinamico::QUANTITATIVO == $tipo)
        {
            return $requisicaoQuantitativo;
        }

        throw new Exception('Tipo de colunas não identificado.');
    }

    public function getFiltrosSelecionados ()
    {
        $copiaConfig = $this->getDadosRequisicao();
        unset(
            $copiaConfig[Simec_ExtratoDinamico::QUALITATIVO],
            $copiaConfig[Simec_ExtratoDinamico::QUANTITATIVO]
        );

        return $copiaConfig;
    }

    protected function getConfig ()
    {
        return $this->config;
    }

    protected function renderJS ()
    {
        $idForm = $this->getId();
        $popupPosfix = self::POPUP_POSFIX;
        $formPosfix = self::FORM_POSFIX;
        $onFormSubmit = 'on' . ucfirst($idForm) . 'Submit';
        $onFormArmazenarSubmit = 'on' . ucfirst($idForm . $formPosfix) . 'Submit';
        $ql = Simec_ExtratoDinamico::QUALITATIVO;
        $qt = Simec_ExtratoDinamico::QUANTITATIVO;

        return <<<JS
<script type="text/javascript">
$(function(){
    $('#{$idForm} .btn-salvar').click(function(){
        $.each($(".form-control.combo.campoEstilo"), function (i, item) {
            $('option', item).prop('selected', true);
        });
        if (null == $('select[name="{$idForm}[{$ql}][]"]').val() && $('select[name="{$idForm}[{$ql}][]"]').length) {
            alert('Ao menos uma coluna qualitativa deve ser informada.');
            return false;
        }
        if (null == $('select[name="{$idForm}[{$qt}][]"]').val() && $('select[name="{$idForm}[{$qt}][]"]').length) {
            alert('Ao menos uma coluna quantitativa deve ser informada.');
            return false;
        }

        $('#{$idForm}{$popupPosfix}').modal();
    });
    
    $(".pagination").click(function(){
        $.each($(".form-control.combo.campoEstilo"), function (i, item) {
            $('option', item).prop('selected', true);
        });
    });
    
    return false;
});

function AgrupadorOrigemParaDestino() {
    $("select[origem]").each(function (index, item){
        $('select[name="' + $(item).attr('destino') + '"]').append($("option", item).clone());
        $("option", item).remove();
    });
}
function AgrupadorDestinoParaOrigem() {
    $("select[destino]").each(function (index, item){
        $('select[name="' + $(item).attr('origem') + '"]').append($("option:not(:selected)", $("select[name='" + $(item).attr('destino') + "']")).clone());
        $("option:not(:selected)", $("select[name='" + $(item).attr('destino') + "']")).remove();
    });
}

function {$onFormSubmit}()
{
    if (null == $('select[name="{$idForm}[{$ql}][]"]').val() && $('select[name="{$idForm}[{$ql}][]"]').length) {
        alert('Ao menos uma coluna qualitativa deve ser informada.');
        return false;
    }
    if (null == $('select[name="{$idForm}[{$qt}][]"]').val() && $('select[name="{$idForm}[{$qt}][]"]').length) {
        alert('Ao menos uma coluna quantitativa deve ser informada.');
        return false;
    }

    var \$opcoesEscolhidas = $('#{$idForm}_{$ql}_chosen .search-choice-close');
    $('#{$idForm}_{$ql} option:selected').removeAttr('selected');

    \$opcoesEscolhidas.each(function(){
        var index = $(this).attr('data-option-array-index');

        $('<input />').attr({
            type:'hidden',
            name:"{$idForm}[{$ql}][]",
            value:$('#{$idForm}_{$ql} option').eq(index).val()
        }).appendTo('#{$idForm}');
    });

    \$opcoesEscolhidas = $('#{$idForm}_{$qt}_chosen .search-choice-close');
    $('#{$idForm}_{$qt} option:selected').removeAttr('selected');

    \$opcoesEscolhidas.each(function(){
        var index = $(this).attr('data-option-array-index');
        $('<input />').attr({
            type:'hidden',
            name:"{$idForm}[{$qt}][]",
            value:$('#{$idForm}_{$qt} option').eq(index).val()
        }).appendTo('#{$idForm}');
    });
}

function {$onFormArmazenarSubmit}()
{
    if ('' == $('#extrato_formarmazenar_edcnome').val()) {
        alert('O nome do relatório não pode ser em branco.');
        return false;
    }

    $('<input />').attr({
        type:'hidden',
        name:'{$idForm}[salvarconsulta][edcnome]',
        value:$('#{$idForm}{$formPosfix}_edcnome').val()
    }).appendTo('#{$idForm}');

    $('<input />').attr({
        type:'hidden',
        name:'{$idForm}[salvarconsulta][edcdescricao]',
        value:$('#{$idForm}{$formPosfix}_edcdescricao').val()
    }).appendTo('#{$idForm}');

    $('<input />').attr({
        type:'hidden',
        name:'{$idForm}[salvarconsulta][edcpublico]',
        value:(1 == $('#extrato_formarmazenar_edcpublico:checked').length)?1:''
    }).appendTo('#{$idForm}');

    $('#{$idForm}').submit();

    return false;
}
</script>
JS;
    }

    /**
     * Normaliza o formulário após ele ser submitido através dos comandos da listagem.
     *
     * @param type $dadosFormulario
     * @return type
     */
    protected function normalizaDadosFormulario ()
    {
        if (!isset($_POST['listagem']))
        {
            return;
        }

        foreach ($this->dadosRequisicao as $item => $valor)
        {
            if (empty($this->dadosRequisicao[$item][0]))
            {
                unset($this->dadosRequisicao[$item]);
                continue;
            }

            $this->dadosRequisicao[$item] = explode(
                ',',
                $this->dadosRequisicao[$item][0]
            );
        }
    }

    protected function renderPopupSalvar ()
    {
        $htmlForm = (new Simec_View_Form($this->getId() . self::FORM_POSFIX))
            ->addTexto('Nome', 'edcnome', 250)
            ->addTextarea('Descrição', 'edcdescricao', 1000)
            ->addCheckbox('Público?', 'edcpublico')
            ->render(false);

        ob_start();
        bootstrapPopup('Salvar consulta', $this->getId() . self::POPUP_POSFIX, $htmlForm, ['cancelar', 'confirmar']);
        $html = ob_get_contents();
        ob_end_clean();

        return $html;
    }
}


