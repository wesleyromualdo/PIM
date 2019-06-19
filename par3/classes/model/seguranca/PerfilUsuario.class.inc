<?php
/**
 * Classe de mapeamento da entidade seguranca.perfilusuario.
 *
 * @version $Id$
 * @since 2016.08.31
 */

/**
 * Seguranca_Model_Perfilusuario: sem descricao
 *
 * @package Seguranca\Model
 * @uses Simec\Db\Modelo
 * @author  <>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Seguranca_Model_Perfilusuario($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Seguranca_Model_Perfilusuario($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $usucpf
 * @property int $pflcod
 */
class Seguranca_Model_Seguranca_PerfilUsuario extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'seguranca.perfilusuario';

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
        'pflcod' => array('tabela' => 'seguranca.perfil', 'pk' => 'pflcod'),
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'usucpf' => null,
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
            'usucpf' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'usucpf' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'pflcod' => array('Digits'),
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
        $this->usucpf = str_replace(array('.', ''), array('-', ''), $this->usucpf);
        return parent::antesSalvar();
    }

    public function salvarPerfil($cpf, $pflcods)
    {

        foreach ($pflcods as $pflcod){
            $sql = " INSERT INTO seguranca.perfilusuario( usucpf, pflcod) VALUES ('{$cpf}', '{$pflcod}') returning usucpf; ";
            return $this->executar($sql, false);
        }
    }

    /**
     * @author Sérgio Henrique <sergio.hsilva@mec.gov.br>
     * @description esta função verifica se o usuário possui perfil do CACS
     * @return boolean
     */
    public function verficaPerfilCacs($cpf){
        $sql = "SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf = '{$cpf}' AND pflcod IN (1503,1504, 1501, 1502)";

        return $this->pegaUm($sql) ? true : false;
    }
}
