<?php

/**
 * Classe que encapsula as funções de manipulação do log do SimecBackup
 *
 * @example
 * <code>
 * $log = new Siemc_Workflow_Log();
 *
 * // Exemplo de mensagens de SUCCESS, WARNING e ERROR.
 * $log->addMensagem('Tudo certo por enquanto');
 * $log->addMensagem('Opa! Tivemos um probleminha mas da pra continuar :D', Siemc_Workflow_Log::WARNING);
 * $log->addMensagem('Vish, deu ruim!', Siemc_Workflow_Log::ERROR);
 *
 * $log->render();
 * </code>
 *
 * Class SimecBackupLog
 */
class Siemc_Workflow_Log {
    const SUCCESS = "background: #dff0d8; border: 1px solid #d6e9c6; color: #3c763d;";
    const WARNING = "background: #fcf8e3; border: 1px solid #faebcc; color: #8a6d3b;";
    const ERROR = "background: #f2dede; border: 1px solid #ebccd1; color: #a94442;";

    private $mensagens = [];
    private $tipoLog = self::SUCCESS;

    /**
     * Função que adiciona uma mensagem ao log que da importação do workflow
     *
     * @param $mensagem
     * @param string $tipo
     * @return $this
     */
    public function addMensagem($mensagem, $tipo = self::SUCCESS){
        $this->mensagens[] = <<<HTML
            <tr>
                <td>
                    <div style="{$tipo} padding: 7px; margin: 6px; border-radius: 4px; text-align: left;">
                        {$mensagem}
                    </div>
                </td>
            </tr>
HTML;
        return $this;
    }

    /**
     *  Apresenta o log
     * @todo Variar a forma de apresentação (HTML, TABELA, etc...)
     */
    public function render(){
        $mensagens = implode("", $this->mensagens);

        $html = <<<HTML
            <table class="tabela" bgcolor="#f5f5f5" cellspacing="0" cellpadding="0" align="center">
                <tbody>
                    <tr>
                        <td width="100%" align="center">
                            <!--<div style="{$this->tipoLog} padding: 10px; margin: 6px; border-radius: 4px; text-align: center;">-->
                                <!--Workflow importado com sucesso!-->
                            <!--</div>-->
                            <label class="TituloTela" style="color: #000;">Resultado da Importação</label>
                        </td>
                    </tr>
                    {$mensagens}
                </tbody>
            </table>
HTML;

        echo $html;
    }
}