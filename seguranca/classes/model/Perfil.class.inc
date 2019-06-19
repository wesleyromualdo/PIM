<?php
/**
 * Classe de mapeamento da entidade seguranca.perfil.
 *
 * @version $Id$
 * @since 2016.03.08
 */

/**
 * Seguranca_Model_Perfil: sem descricao
 *
 * @package Seguranca\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardo.neto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Seguranca_Model_Perfil($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Seguranca_Model_Perfil($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $constantevirtual
 * @property int $modid
 * @property bool $pflpadrao Caso o Sistema possua um perfil padrão que não necessite aprovação do gestor, o simec automaticamente ativa no momento do cadastro o usuário neste perfil - default: false
 * @property bool $pflinddelegar  - default: false
 * @property bool $pflsuporte  - default: false
 * @property bool $pflsuperuser  - default: false
 * @property int $sisid
 * @property string $pfldescricao
 * @property smallint $pflnivel
 * @property string $pflfinalidade
 * @property bool $pflsncumulativo
 * @property string $pflresponsabilidade
 * @property string $pflstatus
 * @property \Datetime(Y-m-d H:i:s) $pfldatafim
 * @property \Datetime(Y-m-d H:i:s) $pfldatainicio
 * @property string $pfldsc
 * @property int $pflcod  - default: nextval('perfil_pflcod_seq'::regclass)
 */
class Seguranca_Model_Perfil extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'seguranca.perfil';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'pflcod',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'modid' => array('tabela' => 'modulo', 'pk' => 'modid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'constantevirtual' => null,
        'modid' => null,
        'pflpadrao' => null,
        'pflinddelegar' => null,
        'pflsuporte' => null,
        'pflsuperuser' => null,
        'sisid' => null,
        'pfldescricao' => null,
        'pflnivel' => null,
        'pflfinalidade' => null,
        'pflsncumulativo' => null,
        'pflresponsabilidade' => null,
        'pflstatus' => null,
        'pfldatafim' => null,
        'pfldatainicio' => null,
        'pfldsc' => null,
        'pflcod' => null,
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
            'constantevirtual' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 200))),
            'modid' => array('allowEmpty' => true,'Digits'),
            'pflpadrao' => array('allowEmpty' => true),
            'pflinddelegar' => array(),
            'pflsuporte' => array('allowEmpty' => true),
            'pflsuperuser' => array('allowEmpty' => true),
            'sisid' => array('allowEmpty' => true,'Digits'),
            'pfldescricao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'pflnivel' => array('allowEmpty' => true),
            'pflfinalidade' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1000))),
            'pflsncumulativo' => array('allowEmpty' => true),
            'pflresponsabilidade' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'pflstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'pfldatafim' => array('allowEmpty' => true),
            'pfldatainicio' => array('allowEmpty' => true),
            'pfldsc' => array(new Zend_Validate_StringLength(array('max' => 100))),
            'pflcod' => array('Digits'),
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

    /**
     * montaSQLCombo()
     *
     *  Monta SQL combo
     *
     * @return bool
     */
    public function montaSQLCombo($sisid)
    {
        $sql = "SELECT
                	pflcod as codigo,
                	pfldsc as descricao
                FROM
                	seguranca.perfil
                WHERE
                	sisid = $sisid";
        return $sql;
    }
    
    public function acessoRapidoListarDadosComboAgrupada($sisid, $sisdiretorio)
    {
        $sql = "SELECT
                    pflcod,
                    pfldsc,
                    sisdiretorio
                FROM
                    seguranca.sistema s
                join seguranca.perfil p on p.sisid = s.sisid and
                                           p.pflstatus = 'A'
                WHERE
                    sisstatus='A' and 
                    (s.sisid IN (".($sisid ? implode(", ", $sisid) : "").") OR s.sisdiretorio='{$sisdiretorio}')
                ORDER BY
                    sisdiretorio, pfldsc";
        $dados = $this->carregar($sql);
        
        return ($dados ? $dados : array());
    }

}
