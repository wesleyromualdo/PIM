<?php
/**
 * Classe de mapeamento da entidade sase.fichamonitoramento.
 *
 * @version $Id$
 * @since 2017.01.10
 */

/**
 * Sase_Model_Fichamonitoramento: sem descricao
 *
 * @package Sase\Model
 * @uses Simec\Db\Modelo
 * @author Victor Martins Machado <victormachado@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Sase_Model_Fichamonitoramento($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Sase_Model_Fichamonitoramento($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property bool $fmtperavalnaoprevisto -- definição para ficha com ano não previsto
 * @property bool $fmtperavalquinquenal -- definição de ficha quinquenal a partir do fmtperavalano1
 * @property string $fmtequipeemail 
 * @property string $fmtequipetelefone 
 * @property string $fmtequipecontato 
 * @property numeric $fmtperavalano4 
 * @property bool $fmtperavalquadrienal 
 * @property \Datetime(Y-m-d H:i:s) $fmtdataalteracao 
 * @property \Datetime(Y-m-d H:i:s) $fmtdatainclusao 
 * @property string $usucpf 
 * @property string $fmtstatus  - default: 'A'::bpchar
 * @property string $fmtequipeatolegal 
 * @property string $fmtequipetecnica 
 * @property string $fmtcomnumanoatolegal 
 * @property string $fmtcomcoordenadora 
 * @property numeric $fmtperavalano3 
 * @property numeric $fmtperavalano2 
 * @property numeric $fmtperavalano1 
 * @property bool $fmtperavaltrianual 
 * @property bool $fmtperavalbianual 
 * @property bool $fmtperavalanual 
 * @property string $fmtpme 
 * @property string $muncod 
 * @property string $estuf 
 * @property int $fmtid  - default: nextval('sase.fichamonitoramento_fmtid_seq'::regclass)
 */
class Sase_Model_Fichamonitoramento extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'sase.fichamonitoramento';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'fmtid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
        'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod'),
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'fmtperavalnaoprevisto' => null,
        'fmtperavalquinquenal' => null,
        'fmtequipeemail' => null,
        'fmtequipetelefone' => null,
        'fmtequipecontato' => null,
        'fmtperavalano4' => null,
        'fmtperavalquadrienal' => null,
        'fmtdataalteracao' => null,
        'fmtdatainclusao' => null,
        'usucpf' => null,
        'fmtstatus' => null,
        'fmtequipeatolegal' => null,
        'fmtequipetecnica' => null,
        'fmtcomnumanoatolegal' => null,
        'fmtcomcoordenadora' => null,
        'fmtperavalano3' => null,
        'fmtperavalano2' => null,
        'fmtperavalano1' => null,
        'fmtperavaltrianual' => null,
        'fmtperavalbianual' => null,
        'fmtperavalanual' => null,
        'fmtpme' => null,
        'muncod' => null,
        'estuf' => null,
        'fmtid' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'fmtperavalnaoprevisto' => array('allowEmpty' => true),
            'fmtperavalquinquenal' => array('allowEmpty' => true),
            'fmtequipeemail' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'fmtequipetelefone' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'fmtequipecontato' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 100))),
            'fmtperavalano4' => array('allowEmpty' => true),
            'fmtperavalquadrienal' => array('allowEmpty' => true),
            'fmtdataalteracao' => array('allowEmpty' => true),
            'fmtdatainclusao' => array('allowEmpty' => true),
            'usucpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'fmtstatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
            'fmtequipeatolegal' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1000))),
            'fmtequipetecnica' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5000))),
            'fmtcomnumanoatolegal' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1000))),
            'fmtcomcoordenadora' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5000))),
            'fmtperavalano3' => array('allowEmpty' => true),
            'fmtperavalano2' => array('allowEmpty' => true),
            'fmtperavalano1' => array('allowEmpty' => true),
            'fmtperavaltrianual' => array('allowEmpty' => true),
            'fmtperavalbianual' => array('allowEmpty' => true),
            'fmtperavalanual' => array('allowEmpty' => true),
            'fmtpme' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'muncod' => array(new Zend_Validate_StringLength(array('max' => 7))),
            'estuf' => array(new Zend_Validate_StringLength(array('max' => 2))),
            'fmtid' => array('Digits'),
        );
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function montaLegendaFichaMonitoramentoExterno($estuf)
    {
        global $db; ?>
        <div id="legendaConteudo" style="display:block;float:right;width:285px;" class="topo_bootstrap_off">
            <div id="tituloLegenda">
                <div style="float:left;"><h5 style="font-size:14px !important;margin:2px !important;margin-left:0px !important;">&nbsp;Legenda: </h5></div>
                <div style="float:right;"></div>
                <div style="clear:both;height:1px;">&nbsp;</div>
            </div>
            <ul>
                <?php

                require_once dirname(str_replace('/Model', '',(str_replace('\Model', '', __FILE__)))) . '/Mapa/Metadados/DadoFichaMonitoramentoExterno.php';
                $classeDados = new DadoFichaMonitoramentoExterno();
                $classeDados->estuf = $estuf;
                $legenda = $classeDados->dadosDaLegenda();
                foreach ($legenda as $value) {
                    echo <<<HTML
                        <li>
                            <table>
                                <tr>
                                    <td style="width:47px;">
                                        <span style="background:{$value['cor']};" class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>{$value['total']}</b>&nbsp;&nbsp;
                                    </td>
                                    <td>{$value['descricao']}</td>
                                </tr>
                            </table>
                        </li>
HTML;
                }
                ?>
            </ul>
        </div>
        <?php
    }
}
