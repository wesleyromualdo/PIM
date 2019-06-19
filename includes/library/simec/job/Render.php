<?php
/**
 * $Id: Render.php 144637 2018-10-05 19:50:21Z rodrigocardoso $
 */


class Simec_Job_Render
{
    /**
     * @var string
     */
    private $id;

    /**
     * @var string
     */
    private $title = 'Tarefas';

    /**
     * @var callable
     */
    private $callbackIniciarJob;

    /**
     * @var bool
     */
    private $mostrarBarraProgresso = true;

    /**
     * @var bool
     */
    private $mostrarMensagens = true;

    /**
     * @var bool
     */
    private $mostrarTarefas = true;

    /**
     * @var string
     */
    private $callbackSucessoJS;

    /**
     * @var string
     */
    private $callbackErroJS;

    /**
     * @var bool
     */
    private $debug;

    /**
     * Simec_Job_Render constructor.
     *
     * @param callable    $callbackIniciarJob
     * @param null|string $id
     * @param null|string $callbackSucessoJS
     * @param null|string $callbackErroJS
     */
    public function __construct(callable $callbackIniciarJob, $id = null, $callbackSucessoJS = null, $callbackErroJS = null, $debug = false)
    {
        $this->id = is_null($id) ? 'simec_job' : $id;

        $this->callbackSucessoJS = $callbackSucessoJS;
        $this->callbackErroJS = $callbackErroJS;
        $this->debug = $debug;

        $this->setCallbackIniciarJob($callbackIniciarJob);
    }

    /**
     * @return $this
     */
    public function esconderBarraProgresso()
    {
        $this->mostrarBarraProgresso = false;

        return $this;
    }

    /**
     * @return $this
     */
    public function esconderMensagens()
    {
        $this->mostrarMensagens = false;

        return $this;
    }

    /**
     * @return $this
     */
    public function esconderTarefas()
    {
        $this->mostrarTarefas = false;

        return $this;
    }

    /**
     * @return $this
     */
    public function ativarDebug()
    {
        $this->debug = true;

        return $this;
    }

    /**
     * @param $callbackErroJS
     *
     * @return $this
     */
    public function setCallbackErroJS($callbackErroJS)
    {
        $this->callbackErroJS = $callbackErroJS;

        return $this;
    }

    /**
     * @param $callbackSucessoJS
     *
     * @return $this
     */
    public function setCallbackSucessoJS($callbackSucessoJS)
    {
        $this->callbackSucessoJS = $callbackSucessoJS;

        return $this;
    }

    public function render()
    {
        $tarefas = $this->mostrarTarefas ? <<<HTML
            <div class="col-xs-12 col-sm-6">
                <div class="panel panel-primary">
                    <div class="panel-heading text-center">{$this->title}</div>
                    <div id="{$this->id}-tarefas-jobs" class="simec-job-tasks"></div>
                </div>
            </div>
HTML
            : '';

        $output = $this->mostrarMensagens ? <<<HTML
            <div class="col-xs-12 col-sm-6">
                <div class="panel panel-primary">
                    <div class="panel-heading text-center">Detalhes da Execução</div>
                    <div id="{$this->id}-output-jobs" class="simec-job-output"></div>
                </div>
            </div>
HTML
            : '';

        $outputJS = $this->debug ? <<<JS
    $('#output', outputItem).html((item['info']['output'].length > item['jlgoutput'].length) ? item['info']['output'] : item['jlgoutput']).append(SimecJobErro(item));
JS
            :
            <<<JS
    $('#output', outputItem).html(item['jlgoutput']).append(SimecJobErro(item));
JS;


        $progress = $this->mostrarBarraProgresso ? <<<HTML
            <div class="row">
                <div id="{$this->id}-progress-jobs" class="col-xs-12 form-group" style="display:none;">
                    <div class="progress" style="border: 1px solid #ddd; background: white;">
                        <div class="progress-bar progress-bar-striped active" role="progressbar"
                             aria-valuenow="0" aria-valuemin="0" aria-valuemax="100">
                        </div>
                    </div>
                </div>
            </div>
HTML
            : '';

        $dados = simec_json_encode($_REQUEST);

        echo <<<HTML
<link rel="stylesheet" type="text/css" href="/includes/simec/job/Simec_Job_Css.css">

<script>
    let SimecTemplate = '{$_SESSION['sislayoutbootstrap']}',
        SimecJobCallBackSuccess = '{$this->callbackSucessoJS}',
        SimecJobCallBackError = '{$this->callbackErroJS}',
        SimecJobTime = 15000,
        SimecJobZendId,
        SimecJobData = {$dados},
        SimecJobHost;

    function SimecJobStatusIcon(status) {
        switch (status) {
            case 'Pendente':
            case 'Aguardando Predecessor':
            case 'Agendado':
            case 'Suspenso':
                return $('<i>');

            case 'Em execução':
                if (SimecTemplate == 'zimec')
                    return $('<i>', {
                        class: 'fa fa-spinner fa-spin'
                    });
                else
                    return $('<i>', {
                        class: 'glyphicon glyphicon-repeat spin'
                    });


            case 'Finalizado':
            case 'OK':
                if (SimecTemplate == 'zimec')
                    return $('<i>', {
                        class: 'fa fa-check',
                        style: 'color: green;'
                    });
                else
                    return $('<i>', {
                        class: 'glyphicon glyphicon-ok',
                        style: 'color: green;'
                    });

            case 'Falhou':
            case 'Falhou Logicamente':
            case 'Tempo Esgotado':
            case 'Removido':
                if (SimecTemplate == 'zimec')
                    return $('<i>', {
                        class: 'fa fa-exclamation-triangle',
                        style: 'color: red;'
                    });
                else
                    return $('<i>', {
                        class: 'glyphicon glyphicon-exclamation-sign',
                        style: 'color: red;'
                    });
        }
    }

    function SimecJobIniciar() {
        $("#{$this->id}-btn").prop('disabled', true).text('Processando');

        SimecJobData.{$this->id} = {acao: 'Simec_Job_Iniciar'};

        $.ajax({
            type: 'POST',
            data: SimecJobData,
            dataType: 'json',
            success: function (result) {
                let jobid = result['Simec_Job_Id'];

                $('#{$this->id}-tarefas-jobs').html('');
                $('#{$this->id}-output-jobs').html('').height($(window).height() * 0.5);
                $('#{$this->id}-progress-jobs').hide('fade');

                SimecJobZendId = jobid;
                SimecJobHost = result['Simec_Job_Host'];

                SimecJobStatus(jobid, SimecJobHost);
            }
        });
    }

    function SimecJobProgress(qtdJobs, qtdConcluida) {
        let porcentagem = ((qtdConcluida / qtdJobs) * 100).toFixed(0),
            espacamento = 0,
            progress = $("#{$this->id}-progress-jobs");


        if (SimecTemplate != 'zimec')
            espacamento = 4;

        progress.show();
        $(".progress-bar", progress)
            .addClass('active')
            .addClass('progress-bar-striped')
            .removeClass('progress-bar-danger')
            .removeClass('progress-bar-success')
            .attr('aria-valuenow', porcentagem)
            .text(porcentagem + '%')
            .width((porcentagem - (espacamento * 2)) + '%')
            .css('padding', espacamento + 'px');

        if (porcentagem == '100')
            $(".progress-bar", progress)
                .addClass('progress-bar-success')
                .removeClass('active')
                .removeClass('progress-bar-striped');
    }

    function SimecJobProgressStop() {
        $(".progress-bar.progress-bar-striped.active", $("#{$this->id}-progress-jobs"))
            .removeClass('active')
            .removeClass('progress-bar-striped')
            .addClass('progress-bar-danger');
    }

    function SimecJobTitle(id, erase) {
        let div = $('#{$this->id}-' + id);

        if (erase)
            div.html('');

        return div;
    }

    function SimecJobStatus(jobid, host) {
        SimecJobData.{$this->id} = {acao: 'Simec_Job_Status', Simec_Job_Id: jobid, Simec_Job_Host: host};

        $.ajax({
            type: 'POST',
            data: SimecJobData,
            dataType: 'json',
            global: false,
            success: function (dados) {
                $("#loading").hide('fade');
                $("#{$this->id}").show('fade');
                $("#{$this->id}-painel").show('fade');

                let divTarefas = SimecJobTitle('tarefas-jobs', true),
                    divOutput = SimecJobTitle('output-jobs'),
                    ultimoStatus = '',
                    quantidadeConcluida = 0;

                $.each(dados, function (index, item) {
                    ultimoStatus = item['status'];

                    if (item['status'] == 'Finalizado' || item['status'] == 'OK')
                        quantidadeConcluida++;

                    divOutput.append(SimecJobOutput(item));

                    divTarefas.append(SimecJobTarefas(item));
                });

                SimecJobProgress(dados.length, quantidadeConcluida);

                divOutput.animate({ scrollTop: divOutput.prop("scrollHeight")}, 500);

                if (ultimoStatus != 'Finalizado' && ultimoStatus != 'OK' && ultimoStatus != 'Falhou' && ultimoStatus != 'Falhou Logicamente') {
                    setTimeout(function () {
                        SimecJobStatus(jobid, host);
                    }, SimecJobTime);
                } else if (ultimoStatus == 'Falhou' || ultimoStatus == 'Falhou Logicamente') {
                    SimecJobProgressStop();
                    SimecJobExecuteCallback(SimecJobCallBackError);
                } else {
                    SimecJobExecuteCallback(SimecJobCallBackSuccess);
                }

                $('#{$this->id}-tarefas-jobs').show('fade');
                $('#{$this->id}-output-jobs').show('fade');
                $('#{$this->id}-log-jobs').show('fade');
            },
            error : function () {
                setTimeout(function () {
                    SimecJobStatus(jobid, host);
                }, SimecJobTime);
            }
        });
    }

    function SimecJobExecuteCallback(callbackstring) {
        $("#{$this->id}-btn").prop('disabled', false).text('Iniciar Processamento');

        try {
            if (callbackstring)
                eval(callbackstring)();
            else 
                alert('Processo Finalizado!');
        } catch(e) {
            console.log(e);
        }
    }

    function SimecJobDiv(recurso, item) {
        return $("<div>", {
            id: '{$this->id}-' + recurso + '-zendJob-' + item['jlgzendid'],
            class: 'striped panel-body'
        });
    }

    function SimecJobTarefas(item) {
        return SimecJobDiv('tarefa', item)
            .append($('<div>', {class: 'col-xs-1'}).append(SimecJobStatusIcon(item['status'])))
            .append($('<div>', {class: 'col-xs-11'}).append(
                (item['status'] == 'Em execução' ||
                    item['status'] == 'Falhou' ||
                    item['status'] == 'Falhou Logicamente' ||
                    item['status'] == 'Tempo Esgotado' ||
                    item['status'] == 'Removido') ? '<b>' + item['label'] + '</b>' : item['label']))
            .append($('<div>', {style: 'clear: both;'}));
    }

    function SimecJobOutput(item) {
        if (item['jlgoutput'] || item['jlgerror']){
            let outputItem = $('#{$this->id}-output-zendJob-' + item['jlgzendid']);

            if (!outputItem.length)
                return SimecJobDiv('output', item)
                    .append($('<div>', {class: 'text-left', style: 'font-weight: bold;'}).append(item['label']))
                    .append($('<div>', {id: 'output', class: 'col-xs-12'}).append(item['jlgoutput']))
                    .append(SimecJobErro(item))
                    .append($('<div>', {style: 'clear: both;'})).hide().show('fade');
             else {
                {$outputJS}
            }
        }
    }

    function SimecJobErro(item) {
        if (item['jlgerror'])
            return $('<div>', {id: 'erro', class: 'col-xs-12 simec-job-error'}).append(item['jlgerror']);
    }

    function SimecJobDownloadErro(arqid, esquema) {
        window.location.href = "/seguranca/downloadFile.php?download=S&esquema=" + esquema + "&arqid=" + arqid;
    }
</script>

<div id="{$this->id}" class="col-xs-12 simec-job-painel">
    <div id="{$this->id}-jobs" class="col-xs-12">
            <div class="form-group">
                <button id="{$this->id}-btn" onclick="SimecJobIniciar()" class="btn btn-success dim">Iniciar Processamento</button>
            </div>
            {$progress}
        <div id="{$this->id}-painel" class="row" style="display: none;">
            {$tarefas}
            {$output}
        </div>
        <div style="clear:both"></div>
    </div>
    <div style="clear:both"></div>
</div>
HTML;

    }

    /**
     * Setar o titulo do job
     *
     * @param string $cgtdsc
     */
    public function setTitle($title)
    {
        $this->title = $title;

        return $this;
    }

    /**
     * @param callable $callback
     *
     * @throws Exception
     */
    private function setCallbackIniciarJob($callback)
    {
        if (is_callable($callback)) {
            $this->callbackIniciarJob = $callback;

            $this->verificarAjax();
        } else {
            throw new Exception("O método '{$callback}' não é um callback executável válido.");
        }
    }

    /**
     * @throws Exception
     */
    private function verificarAjax()
    {
        if (!empty($_REQUEST[$this->id]['acao'])) {
            switch ($_REQUEST[$this->id]['acao']) {
                case 'Simec_Job_Iniciar':
                    {
                        $id = call_user_func($this->callbackIniciarJob);

                        if (empty($id)) {
                            throw new Exception("O método '{$this->callbackIniciarJob}' precisa obrigatoriamente retornar o id do Zend Job.");
                        }

                        ob_clean();
                        die(simec_json_encode(['Simec_Job_Id' => $id, 'Simec_Job_Host' => $_ENV['HOSTNAME']]));
                    }
                case 'Simec_Job_Status':
                    {
                        if ($_ENV['HOSTNAME'] != $_REQUEST[$this->id]['Simec_Job_Host']) {
                            $curl = curl_init();

                            curl_setopt($curl, CURLOPT_URL, "http://{$_REQUEST[$this->id]['Simec_Job_Host']}/includes/simec/job/Simec_Job_Status.php");
                            curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
                            curl_setopt($curl, CURLOPT_POST, 1);
                            curl_setopt($curl, CURLOPT_POSTFIELDS, http_build_query($_REQUEST[$this->id]));

                            $server_output = curl_exec($curl);

                            curl_close($curl);

                            ob_clean();
                            die($server_output);
                        } else {
                            ob_clean();
                            die(simec_json_encode(Simec_Job_Manager::getJobsInfoByIdPai($_REQUEST[$this->id]['Simec_Job_Id'], $_REQUEST[$this->id]['Simec_Job_Host'])));
                        }
                    }
            }
        }
    }
}